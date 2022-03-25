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
    // If 'def' define it globally, If 'put' define it locally
    if (strcmp(func, "def") == 0) {
      lenv_def(e, syms->cell[i], a->cell[i+1]);
    }
    if (strcmp(func, "=") == 0) {
      lenv_put(e, syms->cell[i], a->cell[i+1]);
    } 
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
    
    // Pop next argument from list
    lval* val = lval_pop(a, 0);

    // Bind copy into function's environment
    lenv_put(f->env, sym, val);

    // Delete symbol and value
    lval_del(sym); lval_del(val);
  }
  
  // Argument list is now bound so can be cleaned up
  lval_del(a);
  
  // If '&' remains in formal list, bind to empty list
  if (f->formals->count > 0 && strcmp(f->formals->cell[0]->sym, "&") == 0) {
    
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
  
  // If all formals have been bound, then evaluate
  if (f->formals->count == 0) {  
    f->env->par = e;    
    return builtin_eval(f->env, lval_add(lval_sexpr(), lval_copy(f->body)));
  }
  else
  {
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
  if (v->count == 1) {
    return lval_eval(e, lval_take(v, 0));
  }
  
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

// If tagged as parser, return contents
lval* lval_read(mpc_ast_t* t) {
  
  if (strstr(t->tag, "number")) {
    return lval_read_num(t);
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