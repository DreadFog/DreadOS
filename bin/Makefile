CC= gcc
CFLAGS= -m32 -std=c99 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Werror -I../include

LD= ld.bfd
LDFLAGS = -melf_i386

AS= gcc
ASFLAGS= -m32 -DASSEMBLER -g -Iinclude

OBJCOPY= objcopy

AR= ar

OBJECTS = $(addsuffix .o, $(basename  $(wildcard *.c)))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

app.o:  $(OBJECTS)
	$(LD) $(LDFLAGS) -r -o app.o $(OBJECTS)

clean:
	rm -f *.o *~ *.a
