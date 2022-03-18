#include <stdio.h>
#include <stdlib.h>

#include "mpc.h"

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

// Parser Declariations 
mpc_parser_t* Number; 
mpc_parser_t* Symbol; 
mpc_parser_t* String; 
mpc_parser_t* Comment;
mpc_parser_t* Sexpr;  
mpc_parser_t* Qexpr;  
mpc_parser_t* Expr; 
mpc_parser_t* Lispy;

// Forward Declarations 
struct lval;
struct lenv;
typedef struct lval lval;
typedef struct lenv lenv;

// Create Enumeration of Lisp Values
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_STR, LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };
       
typedef lval*(*lbuiltin)(lenv*, lval*);

// Built our struct value "lval" (Lisp Value)
struct lval {
  int type;

  // Basic
  long num;
  char* err;
  char* sym;
  char* str;
  
  // Function
  lbuiltin builtin;
  lenv* env;
  lval* formals;
  lval* body;
  
  // Expression
  int count;
  lval** cell;
};

// Construct pointer to a number lval
lval* lval_num(long x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v;
}

// Construct pointer to a error lval
lval* lval_err(char* fmt, ...) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_ERR;  
  va_list va;
  va_start(va, fmt);  
  v->err = malloc(512);  
  vsnprintf(v->err, 511, fmt, va);  
  v->err = realloc(v->err, strlen(v->err)+1);
  va_end(va);  
  return v;
}

// Construct pointer to a symbol lval 
lval* lval_sym(char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->sym = malloc(strlen(s) + 1);
  strcpy(v->sym, s);
  return v;
}

// Construct pointer to a string lval
lval* lval_str(char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_STR;
  v->str = malloc(strlen(s) + 1);
  strcpy(v->str, s);
  return v;
}

lval* lval_builtin(lbuiltin func) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_FUN;
  v->builtin = func;
  return v;
}

lenv* lenv_new(void);

// Bulid new environment for lbuiltin function
lval* lval_lambda(lval* formals, lval* body) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_FUN;

  // Set builtin to Null
  v->builtin = NULL;

  // Build new environment
  v->env = lenv_new();

  // Set formals and body
  v->formals = formals;
  v->body = body;
  return v;  
}

// Pointer to empty special expression level
lval* lval_sexpr(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

// Pointer to new empty Quoted Expression
lval* lval_qexpr(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_QEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void lenv_del(lenv* e);

// Deletes lval* values
void lval_del(lval* v) {

  switch (v->type) {
    case LVAL_NUM: break;
    case LVAL_FUN: 
      if (!v->builtin) {
        lenv_del(v->env);
        lval_del(v->formals);
        lval_del(v->body);
      }
    break;
    case LVAL_ERR: free(v->err); break;
    case LVAL_SYM: free(v->sym); break;
    case LVAL_STR: free(v->str); break;

    // If Special or Quotated Expression found, then delete all elements inside
    case LVAL_QEXPR:
    case LVAL_SEXPR:
      for (int i = 0; i < v->count; i++) {
        lval_del(v->cell[i]);
      }
      free(v->cell);
    break;
  }
  
  free(v);
}

lenv* lenv_copy(lenv* e);

// Copies an lval for putting things into and out of the environment (numbers and functions)
lval* lval_copy(lval* v) {
  lval* x = malloc(sizeof(lval));
  x->type = v->type;
  switch (v->type) {

    // Copy Functions Directly
    case LVAL_FUN:
      if (v->builtin) {
        x->builtin = v->builtin;
      } else {
        x->builtin = NULL;
        x->env = lenv_copy(v->env);
        x->formals = lval_copy(v->formals);
        x->body = lval_copy(v->body);
      }
    break;

    // Copy Numbers Directly
    case LVAL_NUM: x->num = v->num; break;

    // Copy Strings using malloc and strcpy
    case LVAL_ERR: x->err = malloc(strlen(v->err) + 1);
      strcpy(x->err, v->err);
    break;
    case LVAL_SYM: x->sym = malloc(strlen(v->sym) + 1);
      strcpy(x->sym, v->sym);
    break;
    case LVAL_STR: x->str = malloc(strlen(v->str) + 1);
      strcpy(x->str, v->str);
    break;

    // COpy lists by copying each sub-expression
    case LVAL_SEXPR:
    case LVAL_QEXPR:
      x->count = v->count;
      x->cell = malloc(sizeof(lval*) * x->count);
      for (int i = 0; i < x->count; i++) {
        x->cell[i] = lval_copy(v->cell[i]);
      }
    break;
  }
  return x;
}

// Increases the count of the expression list by one, reallocating amount of space.
// Space is used to store extra lval* required.
lval* lval_add(lval* v, lval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

lval* lval_join(lval* x, lval* y) {  
  for (int i = 0; i < y->count; i++) {
    x = lval_add(x, y->cell[i]);
  }
  free(y->cell);
  free(y);  
  return x;
}

// Extracts single element from an S-expression at index 1 and shift lists backward so it doesn't contain lval*
lval* lval_pop(lval* v, int i) {
  // Find item at "i"
  lval* x = v->cell[i];  

  // Shift memory after item at "i" over the top
  memmove(&v->cell[i],
    &v->cell[i+1], sizeof(lval*) * (v->count-i-1));

  // Decrease the count of items in the list
  v->count--;

  // Reallocate memory used
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  return x;
}

// Similar to `lval_pop`, but deletes the list it has extracted element from
lval* lval_take(lval* v, int i) {
  lval* x = lval_pop(v, i);
  lval_del(v);
  return x;
}

void lval_print(lval* v);

// Print lval expressions
void lval_print_expr(lval* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    lval_print(v->cell[i]);    
    if (i != (v->count-1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

// Print lval string
void lval_print_str(lval* v) {
  // Make a Copy of the string 
  char* escaped = malloc(strlen(v->str)+1);
  strcpy(escaped, v->str);
  // Pass it through the escape function 
  escaped = mpcf_escape(escaped);
  // Print it between " characters 
  printf("\"%s\"", escaped);
  // free the copied string 
  free(escaped);
}

// Print an "lval" values onto the output
void lval_print(lval* v) {
  switch (v->type) {
    case LVAL_FUN:
      if (v->builtin) {
        printf("<builtin>");
      } else {
        printf("(\\ ");
        lval_print(v->formals);
        putchar(' ');
        lval_print(v->body);
        putchar(')');
      }
    break;
    case LVAL_NUM:   printf("%li", v->num); break;
    case LVAL_ERR:   printf("Error: %s", v->err); break;
    case LVAL_SYM:   printf("%s", v->sym); break;
    case LVAL_STR:   lval_print_str(v); break;
    case LVAL_SEXPR: lval_print_expr(v, '(', ')'); break;
    case LVAL_QEXPR: lval_print_expr(v, '{', '}'); break;
  }
}

// Print lval followed by a newline
void lval_println(lval* v) { lval_print(v); putchar('\n'); }

// Chows if all fields are equal
int lval_eq(lval* x, lval* y) {
  
  // Different types are always unequal
  if (x->type != y->type) {
    return 0;
  }
  
  // Compare based upon type
  switch (x->type) {
    // Compare number values
    case LVAL_NUM: return (x->num == y->num);

    // Compare string values
    case LVAL_ERR: return (strcmp(x->err, y->err) == 0);
    case LVAL_SYM: return (strcmp(x->sym, y->sym) == 0);
    case LVAL_STR: return (strcmp(x->str, y->str) == 0);

    // If builtins compare, otherwise compare formals and body 
    case LVAL_FUN: 
      if (x->builtin || y->builtin) {
        return x->builtin == y->builtin;
      } else {
        return lval_eq(x->formals, y->formals) && lval_eq(x->body, y->body);
      }

    // If list compare every individual element
    case LVAL_QEXPR:
    case LVAL_SEXPR:
      if (x->count != y->count) {
        return 0;
      }
      for (int i = 0; i < x->count; i++) {
        // If any element not equal, then whole list is not equal
        if (!lval_eq(x->cell[i], y->cell[i])) {
          return 0;
        }
      }
      // Return true after all these checks
      return 1;
    break;
  }
  return 0;
}

// Report type of function was expected
char* ltype_name(int t) {
  switch(t) {
    case LVAL_FUN: return "Function";
    case LVAL_NUM: return "Number";
    case LVAL_ERR: return "Error";
    case LVAL_SYM: return "Symbol";
    case LVAL_STR: return "String";
    case LVAL_SEXPR: return "S-Expression";
    case LVAL_QEXPR: return "Q-Expression";
    default: return "Unknown";
  }
}
