#include "mywc.h"
#include <stdio.h>

// main function
int main(int argc, char *argv[]) {
  const char* filepath = "./text.txt";

  process_file(filepath);
  return 0;
}
