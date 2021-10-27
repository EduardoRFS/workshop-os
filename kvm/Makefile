.PHONY: default
default: all

CC=gcc
LD=ld
OBJCOPY=objcopy
OBJDUMP=objdump
CFLAGS=-g -Wall
VM_CFLAGS=$(CFLAGS) -m32 -nostdlib -fno-stack-protector -fno-PIC
LDFLAGS=-m elf_i386 -e 0x0 -Ttext 0x1000 -Trodata 0x2000 -Tdata 0x3000
EXE=$(CC) $(LDFLAGS)


vm: vm.c
	$(CC) $(CFLAGS) $< -o $@

boot.o: boot.S
	$(CC) $(VM_CFLAGS) -c $< -o $@

helpers.o: helpers.S
	$(CC) $(VM_CFLAGS)  -c $< -o $@

code.o: code.c
	$(CC) $(VM_CFLAGS) -c $< -o $@

tmp.o: boot.o helpers.o code.o
	$(LD) $(LDFLAGS) -static $^ -o $@
	
code.bin: tmp.o
	$(OBJCOPY) --only-section=.text --output-target binary $< $@

rodata.bin: tmp.o
	$(OBJCOPY) --only-section=.rodata --output-target binary $< $@

data.bin: tmp.o
	$(OBJCOPY) --only-section=.data --output-target binary $< $@

disassemble: code.bin
	$(OBJDUMP) -b binary -m i386:x86-64 -D $<

clean:
	rm -f *.o
	rm -f *.bin
	rm -f vm

all: vm code.bin rodata.bin data.bin

run: all
	clear
	./vm
