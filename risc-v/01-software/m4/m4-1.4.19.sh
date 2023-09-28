#!/bin/sh

set -e
mkdir -p build
packagename=m4-1.4.19
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f m4-1.4.19.tar.xz ];then
	wget https://ftp.gnu.org/gnu/m4/m4-1.4.19.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf m4-1.4.19.tar.xz
cd build
../m4-1.4.19/configure --prefix=/
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf m4-1.4.19

