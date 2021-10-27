  .code16
  /* real mode */
boot:
  cli
  lgdt (.Lgdt_descriptor)
  /* enable protected mode */
  mov %cr0, %eax
  or $1, %al
  mov %eax, %cr0
  call $0x08,$entry
  hlt
  
/* GDT */
.Lsetup_gdt:
.Lgdt_start:
.Lgdt_null:
  .4byte 0x0
  .4byte 0x0
gdt_code:
  .2byte 0xffff
  .2byte 0x0
  .byte 0x0
  .byte 0b10011010
  .byte 0b11001111
  .byte 0x0
gdt_data:
  .2byte 0xffff
  .2byte 0x0
  .byte 0x0
  .byte 0b10010010
  .byte 0b11001111
  .byte 0x0
.Lgdt_end:
.Lgdt_descriptor:
  .2byte .Lgdt_end - .Lgdt_start - 1
  .4byte .Lgdt_start
