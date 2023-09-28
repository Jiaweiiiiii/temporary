#!/bin/sh


set -e

mkdir -p build
packagename=libtool-2.4.7
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f libtool-2.4.7.tar.xz ];then
	wget https://mirrors.sjtug.sjtu.edu.cn/gnu/libtool/libtool-2.4.7.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf libtool-2.4.7.tar.xz
cd build
../libtool-2.4.7/configure
make 2>&1 | tee make.out
make install
ldconfig
cd ..
rm -rf build
rm -rf libtool-2.4.7

