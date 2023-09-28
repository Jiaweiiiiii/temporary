#!/bin/sh

set -e
mkdir -p build
packagename=libidn2-2.3.4
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f libidn2-2.3.4.tar.gz ];then
	wget https://ftp.gnu.org/gnu/libidn/libidn2-2.3.4.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf libidn2-2.3.4.tar.gz
cd build
../libidn2-2.3.4/configure
make 2>&1 | tee make.out
make install
ldconfig
cd ..
rm -rf build
rm -rf libidn2-2.3.4

