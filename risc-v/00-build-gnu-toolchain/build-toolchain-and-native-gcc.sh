#!/bin/bash

set -e

#cd ..
#top_dir=`pwd`
#cd -
#
#if [ ! -d riscv-gnu-toolchain ];then
#	git clone git-local:software/riscv/riscv-gnu-toolchain \
#		-b develop-ingenic-rv32gc-ilp32d-linux-gcc12.2-2022-11-23-native
#fi
#
#cd riscv-gnu-toolchain
#mkdir -p build
#cd build
#../configure --prefix=$top_dir/riscv-gnu-gcc --build=X86_64-linux --host=riscv32-unknown-linux-gnu --target=riscv32-unknown-linux-gnu --with-arch=rv32gc --with-abi=ilp32d
#time make linux-native -j20 2>&1 | tee make.out


curr_dir=`pwd`

if [ ! -d riscv-gnu-toolchain ];then
	git clone git-local:software/riscv/riscv-gnu-toolchain \
		-b develop-ingenic-rv32gc-ilp32d-linux-gcc12.2-2022-11-23-native
fi

cd riscv-gnu-toolchain
mkdir -p build
cd build

../configure --prefix=$curr_dir/riscv-gnu-gcc --build=X86_64-linux --host=riscv32-unknown-linux-gnu --target=riscv32-unknown-linux-gnu --with-arch=rv32gc --with-abi=ilp32d
time make linux-native -j16 2>&1 | tee make.out
