/**
 * Prasing Language from the Prompt with the base Polish Notation Grammar.
 * 
 * When testing, run cc -std=c99 -Wall main.c mpc.c -ledit -lm -o main
 */

#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

#include <editline/readline.h>
#include <editline/history.h>

#ifdef _WIN32

static char buffer[2048];

char* readline(char* prompt){
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = "\0";
  return cpy;
}

#else
#endif

// Enumeration of Possible Error Types
enum {LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM};

// Enumeration of Possible lval Types
enum {LVAL_NUM, LVAL_ERR};

// Create New Lisp Value Struct called `lval`
typedef struct {
  int type;
  long num;
  int err;
} lval;

// Create new number type lval
lval lval_num(long x){
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

// Create new error type lval
lval lval_err(int x){
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

// Print lval
void lval_print(lval v){
  switch(v.type){
    // In case type is a number, print it.
    case LVAL_NUM: 
      printf("%li", v.num);
      break;
    // In case type is invalid
    case LVAL_ERR:
      if (v.err == LERR_DIV_ZERO){
        printf("Error: Division By Zero!");
      }
      else if (v.err == LERR_BAD_OP){
        printf("Error: Invalid Operator!");
      }
      else if (v.err == LERR_BAD_NUM){
        printf("Error: Invalid Number!");
      }
    break;
  }
}

void lval_println(lval v){
  lval_print(v);
  putchar('\n');
}

// Get operator string 
lval eval_op(lval x, char* op, lval y){
  if(x.type == LVAL_ERR){
    return x;
  }
  else if(y.type == LVAL_ERR){
    return y;
  }

  if(strcmp(op, "+") == 0){
    return lval_num(x.num+y.num);
  }
  else if(strcmp(op, "-") == 0){
    return lval_num(x.num-y.num);
  }
  else if(strcmp(op, "-") == 0){
    return lval_num(x.num*y.num);
  }
  else if(strcmp(op, "/") == 0){
    return y.num == 0 ? lval_err(LERR_DIV_ZERO): lval_num(x.num / y.num);
  }
  return lval_err(LERR_BAD_OP);
}

// Evaluate function based on input
lval eval(mpc_ast_t* t){
  // If tagged as number, return it directly
  if(strstr(t->tag, "number")){
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x):lval_err(LERR_BAD_NUM);
  }

  // Operator is always second child
  char *operator = t->children[1]->contents;

  // Store third child in 'x'
  lval x = eval(t->children[2]);

  int i = 3;
  while(strstr(t->children[i]->tag, "expression")){
    x = eval_op(x, operator, eval(t->children[i]));
    i++;
  }
  return x;
}

int main(int argc, char** argv) {

  // Create Parsers: number, operator, expression, and lispy
  mpc_parser_t* num = mpc_new("number");
  mpc_parser_t* opr = mpc_new("operator");
  mpc_parser_t* expr = mpc_new("expression");
  mpc_parser_t* lsp = mpc_new("lispy");

  // Define them with the Polish Notation with Regex Prompt
  mpca_lang(MPCA_LANG_DEFAULT,
  "                                                     \
    number   : /-?[0-9]+/ ;                             \
    operator : '+' | '-' | '*' | '/' ;                  \
    expression     : <number> | '(' <operator> <expression>+ ')' ;  \
    lispy    : /^/ <operator> <expression>+ /$/ ;             \
  ",
  num, opr, expr, lsp);

  // Print Starter Information
  puts("Lisperer Version 1.0");
  puts("Press Ctrl+C to Exit\n");

  // Always take command unless ended
  while (1) {

    // Output tag and take input
    char* input = readline("Lisperer>\n");

    // Add input to history
    add_history(input);

    // Return what the user inputs
    printf("Your Message: %s\n", input);

    // Parse Unit Input
    mpc_result_t r;
    if(mpc_parse("<stdin>", input, lsp, &r)){
      lval result = eval(r.output);
      lval_println(result);
      // mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    }
    else{
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    // Free retrieved input
    free(input);
  }
  return 0;
}