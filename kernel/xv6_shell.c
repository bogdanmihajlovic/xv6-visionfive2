
extern int uartgetc();
extern void uartputc_sync(int);
extern int printf(char*);

int getc(){
    int c;
    do{
      c = uartgetc();
    }while(c == -1);
    return c;
}

char command[256];
int position = 0;
char arguments[6][64];

int argc;

void get_command(){
  int cc;
  char c;
  position = 0;
  while(1){
    cc = getc();
    c = cc;
    
    if(c == '\r' || c == '\n'|| position == 254 ) break;
    
    if(c =='\b') position--;
    else command[position++] = c;
    uartputc_sync(c);
  }
  
  command[position++] = '\0';
}
#define NPROGRAM
char listOfCommands[NPROGRAM][16] = {
    "help"
};

int str_compare(const char *str1, const char *str2) {
    int i = 0;
    while(*str1 && *str2 && (*str1 == *str2) && ++i < 257) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void help(){
    printf("HEEELPP\r\n");
}

int find_command(){
    for(int i = 0; i < 1;i++){
        if(str_compare(listOfCommands[i], arguments[0]) == 0){
            help();
            return i;
        }
    }
    return -1;
}
void execute(){

}

void parse_command(){
    argc = 0;
    int j = 0;
    for(int i = 0;i < position;i++){
        if(command[i] == ' '){
            arguments[argc][j] = '\0';
            argc++;
            j = 0;
            continue;
        }
        arguments[argc][j++] = command[i]; 
    }
    arguments[argc++][j] = '\0';
}


void shell(){
  while(1){
    printf("\r\nxv6_shell > ");
    get_command();
    parse_command();
    find_command();
  }
}