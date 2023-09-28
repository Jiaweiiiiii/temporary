#!/bin/bash

#./qemu-system-riscv32 -machine virt \
#	-bios fw_jump.elf \
#	-kernel Image \
#	-nographic \
#	-m 2048 \
#	-drive file=rootfs.ext2,format=raw,id=hd0 \
#        -device virtio-blk-device,drive=hd0 \
#	-append "root=/dev/vda rw" \
#	-fsdev local,security_model=mapped-xattr,id=fsdev0,path=/disk0/users/jwzhang/work/riscv/shared \
#	-device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare

./qemu-system-riscv32 -machine virt \
	-smp 2 \
	-bios fw_jump.elf \
	-kernel ImageC \
	-nographic \
	-m 512 \
	-drive file=rootfs-ltp-20230203.ext2,format=raw,id=hd0 \
	-device virtio-blk-device,drive=hd0 \
	-append "root=/dev/vda rw" \
	-fsdev local,security_model=mapped-xattr,id=fsdev0,path=/disk0/users/jwzhang/work/riscv/shared \
	-device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare


	#-drive file=rootfs.ext2,format=raw,id=hd0 \
#mkdir /tmp/host_files
#mount -t 9p -o trans=virtio,version=9p2000.L hostshare /tmp/host_files
