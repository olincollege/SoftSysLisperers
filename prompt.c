#include <stdio.h>

// Declare buffer size of 2048 for user input
static char input[2048];

int main(int argc, char** argv) {

  // Print Starter Information
  puts("Lisperer Version 1.0");
  puts("Press Ctrl+C to Exit\n");

  // Always take command unless ended
  while (1) {

    // Output prompt
    fputs("Lisperer> ", stdout);

    // Read line
    fgets(input, 2048, stdin);

    // Return what the user inputs
    printf("Your Message: %s", input);
  }
  return 0;
}