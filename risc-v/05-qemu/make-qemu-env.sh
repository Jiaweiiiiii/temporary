#!/bin/bash
set -e

if [ ! -d qemu ];then
	git clone https://gitlab.com/qemu-project/qemu.git
	cd qemu
	git submodule init
	git submodule update --recursive
else
	cd qemu
fi

mkdir -p build
cd build
#CC="gcc-8" ../configure --enable-trace-backends=simple --enable-debug --enable-kvm --target-list=riscv32-linux-user,riscv32-softmmu
CC="gcc-8" ../configure --enable-kvm --target-list=riscv32-linux-user,riscv32-softmmu
time make -j16 2>&1 | tee make.out
mv qemu-system-riscv32 ../../
cd ../../
cp ../rootfs.ext2 .
./qemu-system-riscv32 -machine virt \
	-bios fw_jump.elf \
	-kernel Image \
	-nographic \
	-m 1024 \
	-drive file=rootfs.ext2,format=raw,id=hd0 \
        -device virtio-blk-device,drive=hd0 \
	-append "root=/dev/vda rw" 


