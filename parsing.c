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


long eval_op(long x, char *op, long y);
long eval(mpc_ast_t *t);

int main(int argc, char** argv) {

  mpc_parser_t* Lint = mpc_new("int");
  mpc_parser_t* Lfloat = mpc_new("float");
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr = mpc_new("expr");
  mpc_parser_t* MyLisp = mpc_new("mylisp");

  mpca_lang(MPCA_LANG_DEFAULT,
      "                                                                    \
        int     : /-?[0-9]+/ ;                                             \
        float   : /-?[0-9]+\\.[0-9]+/ ;                                    \
        number  : <float> | <int> ;                                        \
        operator: '+' | \"add\" | '-' | \"sub\" |                          \
                  '*' | \"mul\" | '/' | \"div\" |                          \
                  '%' | \"res\" ;                                          \
        expr    : <number> | '(' <operator> <expr>+ ')';                   \
        mylisp  : /^/ <operator> <expr>+ /$/ ;                             \
      ",
      Lint, Lfloat, Number, Operator, Expr, MyLisp);


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

      long result = eval(r.output);
      printf("%li\n", result);
      mpc_ast_delete(r.output);

      
    } else {
      /* Otherwise Print the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  mpc_cleanup(6, Lint, Lfloat, Number, Operator, Expr, MyLisp);
  return 0;
}



long eval(mpc_ast_t *t){

  if (strstr(t->tag, "number")){
    return atoi(t->contents);
  }
  
  char *op = t->children[1]->contents;
  
  long x = eval(t->children[2]);

  int i = 3;
  while(strstr(t->children[i]->tag, "expr")){
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}


long eval_op(long x, char *op, long y) {
  if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0) { return x + y; }
  if (strcmp(op, "-") == 0 || strcmp(op, "sub") == 0) { return x - y; }
  if (strcmp(op, "*") == 0 || strcmp(op, "mul") == 0) { return x * y; }
  if (strcmp(op, "/") == 0 || strcmp(op, "div") == 0) { return x / y; }
  if (strcmp(op, "%") == 0 || strcmp(op, "res") == 0) { return x % y; }
  return 0;
}
