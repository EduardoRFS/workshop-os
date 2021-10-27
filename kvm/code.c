#include "./boot.h"
#include "./helpers.h"

int puts(const char* string) {
  for (int i = 0; string[i] != '\0'; i++) {
    putchar(string[i]);
  }
  putchar('\n');
  return 0;
}
int entry() {
  // load_idt();
  puts("hello");
  return 0;
}
