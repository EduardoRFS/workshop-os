#include "./boot.h"
#include "./helpers.h"

int puts(const char* string) {
  for (int i = 0; string[i] != '\0'; i++) {
    putchar(string[i]);
  }
  putchar('\n');
  return 0;
}
void entry() {
  puts("hello");
  int fd = open("./tuturu.txt", O_RDWR);
  if (fd == -1) {
    exit(1);
  }
  write(fd, "hello twice 3\n", 15);
}
