#!/bin/bash
set -e

IMG_SIZE=131072
if [ $# -eq 2 ];then
	let IMG_SIZE*=$2
	TOTAL_SIZE=0
	let TOTAL_SIZE=8*1024*${IMG_SIZE}/1024/1024/1024
	echo "TOTAL_SIZE=${TOTAL_SIZE}GB"
fi

########### create 1gb empty file ##########################
dd if=/dev/zero of=./add.img bs=8k count=${IMG_SIZE}

########### append empty file to exist ext2 image file #####
#### cat ./add.img >> rootfs.ext2
cat ./add.img >> ./$1

########### verify ext2 image file #########################
#### e2fsck -f ./rootfs.ext2
e2fsck -f ./$1

########### resize ext2 image file #########################
resize2fs ./$1
