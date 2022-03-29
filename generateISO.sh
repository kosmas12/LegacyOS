#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/LegacyOS.elf isodir/boot/LegacyOS.elf
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Legacy OS" {
	multiboot /boot/LegacyOS.elf
}
EOF
grub-mkrescue -o LegacyOS.iso isodir
