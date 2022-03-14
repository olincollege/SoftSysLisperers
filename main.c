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

// Get operator string 
long eval_op(long x, char* op, long y){
  if(strcmp(op, "+") == 0){
    return x + y;
  }
  else if(strcmp(op, "-") == 0){
    return x - y;
  }
  else if(strcmp(op, "-") == 0){
    return x * y;
  }
  else if(strcmp(op, "/") == 0){
    return x / y;
  }
  return 0;
}

// Evaluate function based on input
long eval(mpc_ast_t* t){
  // If tagged as number, return it directly
  if(strstr(t->tag, "number")){
    return atoi(t->contents);
  }

  // Operator is always second child
  char *operator = t->children[1]->contents;

  // Store third child in 'x'
  long x = eval(t->children[2]);

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
      long result = eval(r.output);
      printf("%li\n", result);
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