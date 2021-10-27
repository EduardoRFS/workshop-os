  .code32
putchar:
  mov 4(%esp), %eax
  mov $0x3f8, %dx
  out %al, (%dx)
  ret
