#!/bin/bash
set -e

if [ ! -d linux ]; then
	git clone git-local:software/riscv/victory2/linux/kernel/stable/linux \
		-b research-v6.0.9-victory2-82dd33fde026-non-coherency
fi

cd linux
export ARCH=riscv CROSS_COMPILE=riscv32-unknown-linux-gnu-
make rv32_defconfig
make -j16
cp ./arch/riscv/boot/Image ../
