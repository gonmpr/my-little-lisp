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

// cc -std=c99 -Wall parsing.c mpc.c -ledit -lm -o parsing



int main(int argc, char** argv) {
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr = mpc_new("expr");
  mpc_parser_t* MyLisp = mpc_new("mylisp");

  mpca_lang(MPCA_LANG_DEFAULT,
      "                                                   \
        number  : /-?[0-9]+/ ;                            \
        operator: '+' | '-' | '*' | '/';                  \
        expr    : <number> | '(' <operator> <expr>+ ')';  \
        mylisp  : /^/ <operator> <expr>+ /$/ ;            \
      ",
      Number, Operator, Expr, MyLisp);


  puts("My-little-lisp Line Interpreter");
  puts("Press Ctrl+c to Exit\n");

  while (1) {

    char* input = readline("MLL> ");
    if (input[0] == '\0'){
      free(input);
      continue;
    }
    add_history(input);

    /* Attempt to Parse the user Input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, MyLisp, &r)) {
      /* On Success Print the AST */
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise Print the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  mpc_cleanup(4, Number, Operator, Expr, MyLisp);
  return 0;
}
