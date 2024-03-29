# LegacyOS

LegacyOS is an operating system meant to run on old systems.
Eventually, this project aims to be a complete operating system,
with drivers, userland, system calls, and generally everything
that a user expects to find.

# Special Thanks
Special thanks to the OSDev wiki for having nice tutorials
and documentation and providing a good reference for
implementing some libc functions!

# System Support

Currently, the only supported architecture is x86. This operating
system is meant for 32-bit BIOS systems. 64-bit BIOS will 
also work, but LegacyOS does not make use of 64-bit 
capabilities (or at least not yet), so it's basically the 
same thing. UEFI will boot, but there will be no video output,
as the video driver is designed with VGA memory in mind, which
does not exist in UEFI computers. However, what you can do
to make this work on UEFI computers is boot in Legacy mode
(this setting can be changed inside the UEFI firmware, but
it is most likely already turned on from the factory).

# Features list
- [x] Video driver
- [x] PS/2 keyboard support
- [x] RTC reading
- [X] RTC writing
- [ ] Storage/filesystem(s)
- [ ] Make proper use of Multiboot
- [X] Shell (in progress)
- [ ] Memory management 

More soon...

# Requirements and building

LegacyOS uses shell scripts and makefiles to create its 
bootable image. Specifically, all you need to do is
run the `generateISO.sh` script to build the OS and
create the image, and then optionally `qemu.sh` to run it 
within the QEMU emulator. All you need is `clang`,`make`
and `nasm` for building, and `grub-mkrescue` along with 
its dependencies (notably `xorriso` and GNU mtools) to create the image.

# License
LegacyOS is licensed under the MIT license. This permits
using this code with pretty much no limitation. See
LICENSE for details.
