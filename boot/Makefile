CC= gcc
CFLAGS= -m32 -std=c99 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I../include

LD= ld
LDFLAGS = -melf_i386

AS= gcc
ASFLAGS= -m32 -DASSEMBLER -g -I../include

OBJCOPY= objcopy

all: crt0.o

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ task_dump_screen.o
