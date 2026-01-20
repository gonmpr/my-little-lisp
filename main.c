#include <stdio.h>


static char input[2048];

int main(int argc, char** argv) {
  

  puts("My-little-lisp Line Interpreter");
  puts("Press Ctrl+c to Exit\n");

  while (1) {

    fputs("MLL> ", stdout);

    if(!fgets(input, 2048, stdin)){
      break;
    }

    printf("RUN: %s", input);
  }

  return 0;
}
