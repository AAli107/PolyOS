#!/bin/sh
set -e
. ./iso.sh

qemu-system-x86_64 -cdrom PolyOS.iso -m 2G -no-reboot
