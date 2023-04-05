DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)
 
CFLAGS?= -O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -target i686-pc-none
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -L../libc -lk
 
ARCHDIR=arch/$(HOSTARCH)
 
include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
 
KERNEL_OBJS=\
kernel.o \
shell/shell.o \
shell/shellcommands/clearscreen.o \
shell/shellcommands/readrtc.o \
shell/shellcommands/writertc.o \
shell/shellcommands/printhelp.o \
shell/shellcommands/floppylist.o \
$(KERNEL_ARCH_OBJS)

OBJS=\
$(KERNEL_OBJS)
 
LINK_LIST=\
$(LDFLAGS) \
$(KERNEL_OBJS) \
$(LIBS)
 
.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .asm
 
all: LegacyOS.elf
 
LegacyOS.elf: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot LegacyOS.elf
 
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
 
.asm.o:
	$(AS) -f elf32 $< -o $@
 
clean:
	rm -f LegacyOS.elf
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-kernel
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-kernel: LegacyOS.elf
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp LegacyOS.elf $(DESTDIR)$(BOOTDIR)
 
-include $(OBJS:.o=.d)
