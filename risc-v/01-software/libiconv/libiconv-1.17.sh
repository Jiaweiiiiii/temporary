#!/bin/sh

set -e
mkdir -p build
packagename=libiconv-1.17
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f libiconv-1.17.tar.gz ];then
	wget https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.17.tar.gz
fi

if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi

tar xaf libiconv-1.17.tar.gz
cd build
../libiconv-1.17/configure
make 2>&1 | tee make.out
make install
ldconfig
cd ..
rm -rf build
rm -rf libiconv-1.17

