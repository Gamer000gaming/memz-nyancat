nasm -o disk.img kernel.asm
qemu-system-i386 -s -fda disk.img
