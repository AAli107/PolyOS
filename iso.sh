#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/PolyOS.kernel isodir/boot/PolyOS.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "PolyOS" {
	multiboot /boot/PolyOS.kernel
}
EOF
grub-mkrescue -o PolyOS.iso isodir
