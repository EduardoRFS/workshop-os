.PHONY: default
default: all

CC=gcc
LD=ld
OBJCOPY=objcopy
OBJDUMP=objdump
CFLAGS=-g -Wall
LDFLAGS=
EXE=$(CC) $(LDFLAGS)


vm: vm.c
	$(CC) $(CFLAGS) $< -o $@

utils.o: utils.S
	$(CC) $(CFLAGS) -c $< -o $@

code.o: code.c
	$(CC) $(CFLAGS) -c $< -o $@

tmp.o: utils.o code.o
	$(LD) -e 0x0 -static $^ -o $@
	
code.bin: tmp.o
	$(OBJCOPY) --only-section=.text --output-target binary $< $@

disassemble: code.bin
	$(OBJDUMP) -b binary -m i386:x86-64 -D $<

clean:
	rm -f *.o
	rm -f *.bin
	rm -f vm

all: vm code.bin 

run: all
	./vm
