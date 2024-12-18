#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_prompt()
{
  printf("> ");
  fflush(stdout); // Ensure prompt is displayed immediately
}

void process_input(const char* input)
{
  if (strncmp(input, "exit", 4) == 0)
  {
    printf("Exiting...\n");
    fflush(stdout);
    emscripten_force_exit(0);
  }
  else
  {
    printf("You entered: %s\n", input);
    fflush(stdout);
  }
}
