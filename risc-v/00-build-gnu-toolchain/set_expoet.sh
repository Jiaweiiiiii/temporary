#!/bin/bash
set -e

TOP=$(pwd)

if [ ! -d riscv-gnu-gcc ];then
	./build-toolchain-and-native-gcc.sh
fi

cd $TOP/riscv-gnu-gcc/bin
CORSS_TOOLCHAIN=$(pwd)
export PATH=$CORSS_TOOLCHAIN:$PATH

