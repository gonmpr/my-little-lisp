#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

char *readline(char *prompt){

  fputs(prompt, stdout);

  if(!fgets(buffer, 2048, stdin)){
    return NULL;
  }

  size_t buff_size = strlen(buffer);
  
  char *cpy = malloc(buff_size + 1);

  strcpy(cpy, buffer);

  return cpy;
}

void add_history(char *unused){}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

// cc -Wall -Wextra -std=c99 main.c -ledit -o mll




int main(int argc, char** argv) {
  

  puts("My-little-lisp Line Interpreter");
  puts("Press Ctrl+c to Exit\n");

  while (1) {

    char* input = readline("MLL> ");
    if (input[0] == '\0'){
      free(input);
      continue;
    }

    add_history(input);

    printf("RUN: %s\n", input);

    free(input);
  }

  return 0;
}
