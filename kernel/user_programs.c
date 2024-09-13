#include "user_programs.h"
extern int printf(char*);

int
echo(int argc, char* argv[])
{
  int i;

  for(i = 1; i < argc; i++){
    printf(argv[i]);
    if(i + 1 < argc){
      printf(" ");
    } else {
      printf("\r\n");
    }
  }
  return 0;
}

int
help(int args, char* argv[]){
    if(args > 1){
        printf("\r\nCommand 'help' does not have arguments");
    }else{
        printf("\r\nCommands:");
        printf("\r\nhelp");
        printf("\r\necho");
        printf("\r\nwc");
    }
    return 0;
}


void int_to_str(int n, char* str){
    int i = 0;
    int is_negative = 0;

    if (n < 0) {
        is_negative = 1;
        n = -n; 
    }

    do {
        str[i++] = (n % 10) + '0'; 
        n /= 10;
    } while (n > 0);

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    int len =  i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = temp;
    }
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

int
wc(int argc, char *argv[])
{
  if(argc != 2){
    printf("\r\nwc has just 1 argumen\r\n");
    return 1;
  }

  int i;
  int l, w, c, inword;

  i = l = w = c = inword = 0;
  char* buf = argv[1];
  
  while(buf[i] != '\0'){
    c++;
    if(buf[i] == '\n' || buf[i] == '\r') l++;
    if(strchr(" \r\t\n\v", buf[i])) inword = 0;
    else if(!inword){
        w++;
        inword = 1;
    }
    i++;
  }

  int numbers[3] = {l, w, c};
  for(int i = 0;i < 3;i++){
    char str_int[12];
    int_to_str(numbers[i], str_int);
    printf(str_int);
    printf(" ");
  }
  printf("\r\n");
  return 0;
}


char buf[1024];
int match(char*, char*);

int
grep(int argc, char *argv[])
{
  
  if(argc < 3){
    printf("\r\ngrep requires a pattern and a string\r\n");
    return 1;
  }
  
  char *pattern = argv[1]; 
  char *text = argv[2];  
  char *p = text;
  char *q;

  while(*p != '\0'){  
    q = p;

    while(*q != '\0' && *q != '\n')
      q++;

    char saved = *q;
    *q = '\0';

    if(match(pattern, p)){
      printf(p);
      if(saved == '\n') printf("\n");
    }

    *q = saved;
    if(saved == '\n') p = q + 1;
    else break;
  }
  return 0;
}



// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9, or
// https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
  if(re[0] == '^')
    return matchhere(re+1, text);
  do{  // must look at empty string
    if(matchhere(re, text))
      return 1;
  }while(*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[1] == '*')
    return matchstar(re[0], re+2, text);
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
  do{  // a * matches zero or more instances
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  return 0;
}

