#!/bin/sh

set -e
mkdir -p build
packagename=libunistring-1.1
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f libunistring-1.1.tar.gz ];then
	wget https://ftp.gnu.org/gnu/libunistring/libunistring-1.1.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf libunistring-1.1.tar.gz
cd build
../libunistring-1.1/configure
make 2>&1 | tee make.out
make install
ldconfig
cd ..
rm -rf build
rm -rf libunistring-1.1
