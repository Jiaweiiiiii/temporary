#!/bin/bash
set -e

if [ ! -d opensbi ];then
	git clone https://github.com/riscv-software-src/opensbi.git
	cd opensbi
else
	cd opensbi
fi
export CROSS_COMPILE=riscv32-unknown-linux-gnu-
make PLATFORM=generic
cp build/platform/generic/firmware/fw_jump.elf ..
