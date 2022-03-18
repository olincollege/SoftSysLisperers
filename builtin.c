#include <stdio.h>
#include <stdlib.h>

#include "lenv.c"

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

lval* builtin_add(lenv* e, lval* a) { return builtin_op(e, a, "+"); }
lval* builtin_sub(lenv* e, lval* a) { return builtin_op(e, a, "-"); }
lval* builtin_mul(lenv* e, lval* a) { return builtin_op(e, a, "*"); }
lval* builtin_div(lenv* e, lval* a) { return builtin_op(e, a, "/"); }

// Adds built-in variables into the environment
lval* builtin_var(lenv* e, lval* a, char* func) {
  LASSERT_TYPE(func, a, 0, LVAL_QEXPR);
  
  lval* syms = a->cell[0];
  for (int i = 0; i < syms->count; i++) {
    LASSERT(a, (syms->cell[i]->type == LVAL_SYM),
      "Function '%s' cannot define non-symbol. "
      "Got %s, Expected %s.",
      func, ltype_name(syms->cell[i]->type), ltype_name(LVAL_SYM));
  }
  
  LASSERT(a, (syms->count == a->count-1),
    "Function '%s' passed too many arguments for symbols. "
    "Got %i, Expected %i.",
    func, syms->count, a->count-1);
    
  for (int i = 0; i < syms->count; i++) {
    if (strcmp(func, "def") == 0) { lenv_def(e, syms->cell[i], a->cell[i+1]); }
    if (strcmp(func, "=")   == 0) { lenv_put(e, syms->cell[i], a->cell[i+1]); } 
  }
  
  lval_del(a);
  return lval_sexpr();
}

// Make functions act like a built-in
lval* builtin_def(lenv* e, lval* a) {
  return builtin_var(e, a, "def");
}

// Put the built-in's into the environment
lval* builtin_put(lenv* e, lval* a) {
  return builtin_var(e, a, "=");
}

lval* builtin_ord(lenv* e, lval* a, char* op) {
  LASSERT_NUM(op, a, 2);
  LASSERT_TYPE(op, a, 0, LVAL_NUM);
  LASSERT_TYPE(op, a, 1, LVAL_NUM);
  
  int r;
  if (strcmp(op, ">")  == 0) { r = (a->cell[0]->num >  a->cell[1]->num); }
  if (strcmp(op, "<")  == 0) { r = (a->cell[0]->num <  a->cell[1]->num); }
  if (strcmp(op, ">=") == 0) { r = (a->cell[0]->num >= a->cell[1]->num); }
  if (strcmp(op, "<=") == 0) { r = (a->cell[0]->num <= a->cell[1]->num); }
  lval_del(a);
  return lval_num(r);
}

lval* builtin_gt(lenv* e, lval* a) { return builtin_ord(e, a, ">");  }
lval* builtin_lt(lenv* e, lval* a) { return builtin_ord(e, a, "<");  }
lval* builtin_ge(lenv* e, lval* a) { return builtin_ord(e, a, ">="); }
lval* builtin_le(lenv* e, lval* a) { return builtin_ord(e, a, "<="); }

lval* builtin_cmp(lenv* e, lval* a, char* op) {
  LASSERT_NUM(op, a, 2);
  int r;
  if (strcmp(op, "==") == 0) { r =  lval_eq(a->cell[0], a->cell[1]); }
  if (strcmp(op, "!=") == 0) { r = !lval_eq(a->cell[0], a->cell[1]); }
  lval_del(a);
  return lval_num(r);
}

lval* builtin_eq(lenv* e, lval* a) { return builtin_cmp(e, a, "=="); }
lval* builtin_ne(lenv* e, lval* a) { return builtin_cmp(e, a, "!="); }

// Builtin-in If function
lval* builtin_if(lenv* e, lval* a) {
  LASSERT_NUM("if", a, 3);
  LASSERT_TYPE("if", a, 0, LVAL_NUM);
  LASSERT_TYPE("if", a, 1, LVAL_QEXPR);
  LASSERT_TYPE("if", a, 2, LVAL_QEXPR);
  
  // Mark both expressions as evaluable
  lval* x;
  a->cell[1]->type = LVAL_SEXPR;
  a->cell[2]->type = LVAL_SEXPR;
  
  if (a->cell[0]->num) {
    // If condition is true, then evaluate first expression
    x = lval_eval(e, lval_pop(a, 1));
  } else {
    // Otherwise, evaluate second expression
    x = lval_eval(e, lval_pop(a, 2));
  }
  
  // Delete argument list and return
  lval_del(a);
  return x;
}

lval* lval_read(mpc_ast_t* t);

// Load function that can load and evaluate a file when passed a string of its name
lval* builtin_load(lenv* e, lval* a) {
  LASSERT_NUM("load", a, 1);
  LASSERT_TYPE("load", a, 0, LVAL_STR);
  
  // Parse File given by string name 
  mpc_result_t r;
  if (mpc_parse_contents(a->cell[0]->str, Lispy, &r)) {
    
    // Read contents
    lval* expr = lval_read(r.output);
    mpc_ast_delete(r.output);

    // Evaluate each Expression
    while (expr->count) {
      lval* x = lval_eval(e, lval_pop(expr, 0));
      // If Evaluation leads to error print it
      if (x->type == LVAL_ERR) { lval_println(x); }
      lval_del(x);
    }
    
    // Delete expressions and arguments
    lval_del(expr);    
    lval_del(a);
    
    // Return empty list
    return lval_sexpr();
    
  } else {
    // Get Parse Error as String
    char* err_msg = mpc_err_string(r.error);
    mpc_err_delete(r.error);
    
    // Create new error message using it
    lval* err = lval_err("Could not load Library %s", err_msg);
    free(err_msg);
    lval_del(a);
    
    // Cleanup and return error
    return err;
  }
}

lval* builtin_print(lenv* e, lval* a) {
  
  // Print each argument followed by a space
  for (int i = 0; i < a->count; i++) {
    lval_print(a->cell[i]); putchar(' ');
  }
  
  // Print a newline and delete arguments
  putchar('\n');
  lval_del(a);
  
  return lval_sexpr();
}

lval* builtin_error(lenv* e, lval* a) {
  LASSERT_NUM("error", a, 1);
  LASSERT_TYPE("error", a, 0, LVAL_STR);
  
  // Construct Error from first argument
  lval* err = lval_err(a->cell[0]->str);
  
  // Delete arguments and return
  lval_del(a);
  return err;
}

// Register new builtins
void lenv_add_builtin(lenv* e, char* name, lbuiltin func) {
  lval* k = lval_sym(name);
  lval* v = lval_builtin(func);
  lenv_put(e, k, v);
  lval_del(k); lval_del(v);
}

void lenv_add_builtins(lenv* e) {
  // Variable Functions
  lenv_add_builtin(e, "\\",  builtin_lambda); 
  lenv_add_builtin(e, "def", builtin_def);
  lenv_add_builtin(e, "=",   builtin_put);
  
  // List Functions 
  lenv_add_builtin(e, "list", builtin_list);
  lenv_add_builtin(e, "head", builtin_head);
  lenv_add_builtin(e, "tail", builtin_tail);
  lenv_add_builtin(e, "eval", builtin_eval);
  lenv_add_builtin(e, "join", builtin_join);
  
  // Mathematical Functions 
  lenv_add_builtin(e, "+", builtin_add);
  lenv_add_builtin(e, "-", builtin_sub);
  lenv_add_builtin(e, "*", builtin_mul);
  lenv_add_builtin(e, "/", builtin_div);
  
  // Comparison Functions 
  lenv_add_builtin(e, "if", builtin_if);
  lenv_add_builtin(e, "==", builtin_eq);
  lenv_add_builtin(e, "!=", builtin_ne);
  lenv_add_builtin(e, ">",  builtin_gt);
  lenv_add_builtin(e, "<",  builtin_lt);
  lenv_add_builtin(e, ">=", builtin_ge);
  lenv_add_builtin(e, "<=", builtin_le);
  
  // String Functions 
  lenv_add_builtin(e, "load",  builtin_load); 
  lenv_add_builtin(e, "error", builtin_error);
  lenv_add_builtin(e, "print", builtin_print);
}

// Calls functions within the environment (with error checking)
lval* lval_call(lenv* e, lval* f, lval* a) {
  
  // If builtin then simply call that
  if (f->builtin) {
    return f->builtin(e, a);
  }
  
  // Record Argument counts
  int given = a->count;
  int total = f->formals->count;
  
  // While arguments still remain to be processed
  while (a->count) {
    
    // If we run out of formal arguments to bind, then return error
    if (f->formals->count == 0) {
      lval_del(a);
      return lval_err("Function passed too many arguments. "
        "Got %i, Expected %i.", given, total); 
    }
    
    // Pop first symbol from the formals
    lval* sym = lval_pop(f->formals, 0);
    
    // Special case to deal with '&' (and)
    if (strcmp(sym->sym, "&") == 0) {
      
      // Ensure '&' is followed by another symbol
      if (f->formals->count != 1) {
        lval_del(a);
        return lval_err("Function format invalid. "
          "Symbol '&' not followed by single symbol.");
      }
      
      // Next formal should be bound to remaining arguments
      lval* nsym = lval_pop(f->formals, 0);
      lenv_put(f->env, nsym, builtin_list(e, a));
      lval_del(sym); lval_del(nsym);
      break;
    }
    
    lval* val = lval_pop(a, 0);    
    lenv_put(f->env, sym, val);    
    lval_del(sym); lval_del(val);
  }
  
  lval_del(a);
  
  // If '&' remains in formal list, bind to empty list
  if (f->formals->count > 0 &&
    strcmp(f->formals->cell[0]->sym, "&") == 0) {
    
    // Check to ensure that & is not passed invalidly
    if (f->formals->count != 2) {
      return lval_err("Function format invalid. "
        "Symbol '&' not followed by single symbol.");
    }
    
    // Pop and delete '&' symbol
    lval_del(lval_pop(f->formals, 0));
    
    // Pop next symbol and create an empty list
    lval* sym = lval_pop(f->formals, 0);
    lval* val = lval_qexpr();

    // Bind them to the environment and delete it
    lenv_put(f->env, sym, val);
    lval_del(sym); lval_del(val);
  }
  
  if (f->formals->count == 0) {  
    f->env->par = e;    
    return builtin_eval(f->env, lval_add(lval_sexpr(), lval_copy(f->body)));
  } else {
    return lval_copy(f);
  }
  
}

// Evaluate S-Expressions
lval* lval_eval_sexpr(lenv* e, lval* v) {
  
  // Evaluate Children
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(e, v->cell[i]);
  }
    // Error Checking
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) {
      return lval_take(v, i);
    }
  }
  
  // Handle Empty Expression
  if (v->count == 0) {
    return v;
  }

  // Handle Single Expression
  if (v->count == 1) { return lval_eval(e, lval_take(v, 0)); }
  
  // Ensure First Element is function after evaluation
  lval* f = lval_pop(v, 0);
  if (f->type != LVAL_FUN) {
    lval* err = lval_err(
      "S-Expression starts with incorrect type. "
      "Got %s, Expected %s.",
      ltype_name(f->type), ltype_name(LVAL_FUN));
    lval_del(f); lval_del(v);
    return err;
  }
  
  // Call builtin with operator to get result
  lval* result = lval_call(e, f, v);
  lval_del(f);
  return result;
}

// Evaluate Special Expressions
lval* lval_eval(lenv* e, lval* v) {
  if (v->type == LVAL_SYM) {
    lval* x = lenv_get(e, v);
    lval_del(v);
    return x;
  }
  if (v->type == LVAL_SEXPR) { return lval_eval_sexpr(e, v); }
  return v;
}

// Read the program and construct lval* that represents it all
lval* lval_read_num(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("Invalid Number.");
}

// Parse strings
lval* lval_read_str(mpc_ast_t* t) {
  // Cut off the final quote character
  t->contents[strlen(t->contents)-1] = '\0';
  // Copy the string missing out the first quote character 
  char* unescaped = malloc(strlen(t->contents+1)+1);
  strcpy(unescaped, t->contents+1);
  // Pass through the unescape function
  unescaped = mpcf_unescape(unescaped);
  // Construct a new lval using the string
  lval* str = lval_str(unescaped);
  // Free the string and return
  free(unescaped);
  return str;
}

// If tagged as parser, return contents
lval* lval_read(mpc_ast_t* t) {
  
  if (strstr(t->tag, "number")) {
    return lval_read_num(t);
  }
  if (strstr(t->tag, "string")) {
    return lval_read_str(t);
  }
  if (strstr(t->tag, "symbol")) {
    return lval_sym(t->contents);
  }
  
  lval* x = NULL;
  if (strcmp(t->tag, ">") == 0) {
    x = lval_sexpr();
  } 
  if (strstr(t->tag, "sexpr"))  {
    x = lval_sexpr();
  }
  if (strstr(t->tag, "qexpr"))  {
    x = lval_qexpr();
  }
  
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->contents, ")") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->contents, "}") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->contents, "{") == 0) {
      continue;
    }
    if (strcmp(t->children[i]->tag,  "regex") == 0) {
      continue;
    }
    if (strstr(t->children[i]->tag, "comment")) {
      continue;
    }
    x = lval_add(x, lval_read(t->children[i]));
  }
  return x;
}