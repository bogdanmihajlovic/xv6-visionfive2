#include "user_programs.h"
#define NPROGRAM 5
#define MAX_INPUT 256
#define MAX_ARGUMENTS 6

extern int uartgetc();
extern void uartputc_sync(int);
extern int printf(char*);

char input_line[256];
int position = 0;
char arguments[6][64];
int argc;

int (*operations[NPROGRAM])(int, char*[]);
int num_chars_on_screen;

void input_echo(char c){
    if(c == '\b' && num_chars_on_screen > 0) {
        uartputc_sync('\b');
        uartputc_sync(' ');
        uartputc_sync('\b');
        num_chars_on_screen--;
    }else if( c != '\b') {
        if(c == '\n' || c == '\r'){
            uartputc_sync('\r');
            uartputc_sync('\n');
            uartputc_sync('>');
            num_chars_on_screen = 0;
        }else{
            uartputc_sync(c);
            num_chars_on_screen++;
        }
    }
}

int getc(){
    int c;
    do{
      c = uartgetc();
    }while(c == -1);
    return c;
}

void get_command(){
  int quoute_mark = 0;
  int cc;
  char c;
  position = 0;
  num_chars_on_screen = 0;
  while(1){
    cc = getc();
    c = cc;
    
    if(c == '\'') quoute_mark = !quoute_mark;

    if(quoute_mark == 1 && position == MAX_INPUT - 3){
        input_line[position++] = '\'';
        break;
    }

    if(quoute_mark == 0 && (c == '\r' || c == '\n'|| position == MAX_INPUT - 2)) break;
    
    if(c == '\b' && position > 0 && input_line[position - 1] == '\''){
        quoute_mark = 0;
    }

    if(c =='\b' && position > 0) position--;
    else if(c != '\b') input_line[position++] = c;
    input_echo(c);
  }

  input_line[position++] = '\0';
}

char listOfCommands[NPROGRAM][16] = {
    "help",
    "echo",
    "wc",
    "grep",
    "exit"
};

int str_compare(const char *str1, const char *str2) {
    int i = 0;
    while(*str1 && *str2 && (*str1 == *str2) && ++i <= MAX_INPUT) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

int find_command(){
    for(int i = 0; i < NPROGRAM;i++){
        if(str_compare(listOfCommands[i], arguments[0]) == 0){
            return i;
        }
    }
    return -1;
}
void execute(int program_id){
    char* args_ptrs[MAX_ARGUMENTS];
    for (int i = 0; i < MAX_ARGUMENTS; i++) {
        args_ptrs[i] = arguments[i];  
    }
    operations[program_id](argc, args_ptrs);
}

void parse_command(){
    argc = 0;
    int j = 0;
    int quoute_mark = 0;

    for(int i = 0;i < position;i++){
        if(input_line[i] == '\'') {
            if(quoute_mark == 1){
                arguments[argc][j] = '\0';
                argc++;
                j = 0;
            }
            quoute_mark = !quoute_mark ;
            continue;
        }


        if(quoute_mark == 0 && input_line[i] == ' '){
            arguments[argc][j] = '\0';
            argc++;
            j = 0;
            continue;
        }

        if(input_line[i] == '\r' || input_line[i] == '\n'){
            arguments[argc][j++] = '\r';
            arguments[argc][j++] = '\n'; 
        }else{
            arguments[argc][j++] = input_line[i]; 
        }
    }
    arguments[argc][j] = '\0';
    if(input_line[position-2] != '\'') argc++;
}


void shell(){
    operations[0] = help;
    operations[1] = echo;
    operations[2] = wc;
    operations[3] = grep;

    while(1){
        printf("\r\nxv6_shell > ");
        get_command();
        if(position == 0){
            continue;
        }
        parse_command();
        int program_id = find_command();
        if(program_id == -1){
            printf("\r\n");
            printf(arguments[0]);
            printf(" is not founded");
            continue;
        }else if(program_id == NPROGRAM - 1){
            printf("\r\n exiting shell... \r\n");
            return;
        }
        printf("\r\n");
        execute(program_id);
    }
}