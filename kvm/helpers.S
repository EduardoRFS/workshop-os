  .code32
  .globl putchar
putchar:
  mov 4(%esp), %eax /* first arg */
  mov $0x3f8, %dx
  out %al, (%dx)
  ret

  .globl write
write:
  lea 4(%esp), %eax /* pointer to first arg */
  mov $122, %dx
  out %eax, (%dx)

  .globl open
open:
  lea 4(%esp), %eax /* pointer to first arg */
  mov $123, %dx
  out %eax, (%dx)
  ret

  .globl exit
exit:
  lea 4(%esp), %eax /* pointer to first arg */
  mov $124, %dx
  out %eax, (%dx)


