#!/bin/sh


set -e

mkdir -p build
packagename=zlib-1.2.13
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f zlib-1.2.13.tar.xz ];then
	wget https://www.zlib.net/zlib-1.2.13.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf zlib-1.2.13.tar.xz
cd build
../zlib-1.2.13/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf zlib-1.2.13

