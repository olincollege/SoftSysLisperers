/**
 * Main file that receives user input and acts as a command line for our Lisperers language.
 * Current Example based on Polish Notation
 * 
 * When testing: run cc -std=c99 -Wall main.c mpc.c -ledit -lm -o main
 */

#include <stdio.h>
#include <stdlib.h>

#include "builtin.c"

#ifdef _WIN32

static char buffer[2048];

char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

// Records the history of inputs so that they can be retrieved with up and down arrows
void add_history(char* unused) {}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {
  
  // Create parsers
  Number  = mpc_new("number");
  Symbol  = mpc_new("symbol");
  String  = mpc_new("string");
  Comment = mpc_new("comment");
  Sexpr   = mpc_new("sexpr");
  Qexpr   = mpc_new("qexpr");
  Expr    = mpc_new("expr");
  Lispy   = mpc_new("lispy");
  
  // Define them with the following language
  mpca_lang(MPCA_LANG_DEFAULT,
    "                                              \
      number  : /-?[0-9]+/ ;                       \
      symbol  : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ; \
      string  : /\"(\\\\.|[^\"])*\"/ ;             \
      comment : /;[^\\r\\n]*/ ;                    \
      sexpr   : '(' <expr>* ')' ;                  \
      qexpr   : '{' <expr>* '}' ;                  \
      expr    : <number>  | <symbol> | <string>    \
              | <comment> | <sexpr>  | <qexpr>;    \
      lispy   : /^/ <expr>* /$/ ;                  \
    ",
    Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, Lispy);
  
  lenv* e = lenv_new();
  lenv_add_builtins(e);
  
  // Interactive Prompt
  if (argc == 1) {

    // Output Starter Information
    puts("Lisperers Version 1.0");
    puts("Press Ctrl+c to Exit\n");

    while (1) {
    
      char* input = readline("Lisperers> ");
      add_history(input);
      
      // Attempt to parse user input
      mpc_result_t r;
      if (mpc_parse("<stdin>", input, Lispy, &r)) {
        // 
        lval* x = lval_eval(e, lval_read(r.output));
        lval_println(x);
        lval_del(x);
        
        mpc_ast_delete(r.output);
      }
      else {
        // Print and delete error
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
      }
      
      // Free dynamically allocated input when done
      free(input);
    }
  }
  
  // Supplied with list of files 
  if (argc >= 2) {
  
    // Loop over each supplied filename (starting from 1)
    for (int i = 1; i < argc; i++) {
      
      // Argument list with a single argument, the filename 
      lval* args = lval_add(lval_sexpr(), lval_str(argv[i]));
      
      // Pass to builtin load and get the result 
      lval* x = builtin_load(e, args);
      
      // If the result is an error be sure to print it
      if (x->type == LVAL_ERR) { lval_println(x); }
      lval_del(x);
    }
  }
  
  lenv_del(e);
  
  // Undefine and delete parsers
  mpc_cleanup(8, Number, Symbol, String, Comment, Sexpr,  Qexpr,  Expr,   Lispy);
  
  return 0;
}