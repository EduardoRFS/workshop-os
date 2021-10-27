#include <stdint.h>

int putchar(int);

int write(int fd, const void* buf, unsigned int size);

#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02
int open(const char* path, int flags);

void exit(int status);
