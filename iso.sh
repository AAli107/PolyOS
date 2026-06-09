#!/bin/sh
set -e
. ./build.sh

# Clone Limine binary release if not already present
if [ ! -d limine ]; then
    git clone https://github.com/limine-bootloader/limine.git --branch=v8.x-binary --depth=1 limine
fi

# Build the limine host utility (needed for bios-install)
make -C limine

mkdir -p isodir/boot/limine
mkdir -p isodir/EFI/BOOT

# Copy kernel and Limine boot config
cp sysroot/boot/PolyOS.kernel isodir/boot/PolyOS.kernel
cp limine.conf isodir/boot/limine/limine.conf

# Copy Limine BIOS and UEFI files
cp limine/limine-bios.sys     isodir/boot/limine/
cp limine/limine-bios-cd.bin  isodir/boot/limine/
cp limine/limine-uefi-cd.bin  isodir/boot/limine/
cp limine/BOOTX64.EFI         isodir/EFI/BOOT/ 2>/dev/null || true
cp limine/BOOTIA32.EFI        isodir/EFI/BOOT/ 2>/dev/null || true

# Build the ISO using xorriso
xorriso -as mkisofs              \
    -b boot/limine/limine-bios-cd.bin \
    -no-emul-boot                \
    -boot-load-size 4            \
    -boot-info-table             \
    --efi-boot boot/limine/limine-uefi-cd.bin \
    -efi-boot-part               \
    --efi-boot-image             \
    --protective-msdos-label     \
    isodir                       \
    -o PolyOS.iso

# Install Limine BIOS boot sector into the ISO
limine/limine bios-install PolyOS.iso