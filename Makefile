.PHONY: default build run clean

default: build

build:
	cd Data && sh make.sh
	nasm -o disk.img kernel.asm

run: build
	qemu-system-i386 -s -fda disk.img -machine pc,pcspk-audiodev=speaker -audiodev driver=sdl,id=speaker

clean:
	rm -f disk.img Data/*.bin Data/*/*.bin
