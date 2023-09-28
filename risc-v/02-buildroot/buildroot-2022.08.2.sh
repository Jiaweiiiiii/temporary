#!/bin/bash

set -e

cd ..
TOP=$(pwd)
cd -
THREAD=$(cat /proc/cpuinfo | grep processor | wc -l)
TARGET=buildroot-2022.08.2

echo "Current processor num $THREAD"

if [ ! -d build ];then
	mkdir build
fi
cd build

if [ ! -d $TARGET ];then
	if [ ! -f "$TARGET.tar.gz" ];then
		echo "Download "$TARGET
		wget https://buildroot.org/downloads/$TARGET.tar.gz
	fi
	
	tar -xaf $TARGET.tar.gz
	CURR_DIR=`pwd`
	sed -i -E "/.*BR2_ROOTFS_OVERLAY*/c\BR2_ROOTFS_OVERLAY=\"${CURR_DIR}\/${TARGET}\/rootfs-overlay\"/" ../qemu_nutlet_riscv32_virt_defconfig
	cp ../qemu_nutlet_riscv32_virt_defconfig ./$TARGET/configs/
	cd $TARGET
	make qemu_nutlet_riscv32_virt_defconfig
else
	cd $TARGET
fi

mkdir -p rootfs-overlay
cp -r $TOP/riscv-gnu-gcc/native/* ./rootfs-overlay/
mkdir -p ./rootfs-overlay/software-porting
cp -r ../../../01-software-porting/* ./rootfs-overlay/software-porting/
unset PERL_MM_OPT
make -j$THREAD | tee make.log
cd $TOP
