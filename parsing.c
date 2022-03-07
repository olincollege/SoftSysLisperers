/**
 * Receives prompt and spits it back out
 * 
 * When testing, run gcc prompt.c -ledit -o prompt
 */

#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

#include <editline/readline.h>
#include <editline/history.h>

int main(int argc, char** argv) {

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

    // Free retrieved input
    free(input);
  }
  return 0;
}