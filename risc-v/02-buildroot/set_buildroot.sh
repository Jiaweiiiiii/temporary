#!/bin/bash
set -e

THREAD=$(cat /proc/cpuinfo | grep processor | wc -l)
echo "Current processor num $THREAD"


TARGET=buildroot-2023.05.3
if [ ! -d $TARGET ];then
	if [ ! -f "$TARGET.tar.gz" ];then
		echo "Download "$TARGET
		wget https://buildroot.org/downloads/$TARGET.tar.gz
	fi
	
	tar -xaf $TARGET.tar.gz
	CURR_DIR=`pwd`
	sed -i -E "/.*BR2_ROOTFS_OVERLAY*/c\BR2_ROOTFS_OVERLAY=\"${CURR_DIR}\/${TARGET}\/rootfs-overlay\"/" ../qemu_riscv32_virt_defconfig
	cp ./qemu_riscv32_virt_defconfig ./$TARGET/configs/
	cd $TARGET
	make qemu_riscv32_virt_defconfig -j8
else
	cd $TARGET
fi

mkdir -p rootfs-overlay
cp -r ../../00-build-gnu-toolchain/riscv-gnu-gcc/native/* ./rootfs-overlay/
mkdir -p rootfs-overlay/software-porting
cp -r ../../01-software/* ./rootfs-overlay/software-porting/
unset PERL_MM_OPT
make -j$THREAD | tee make.log

cp output/images/rootfs.ext2 ../

echo "buildroot has been completed."
