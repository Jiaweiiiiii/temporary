#!/bin/sh

set -e
mkdir -p build
packagename=autoconf-2.71
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f autoconf-2.71.tar.gz ];then
	wget https://ftp.gnu.org/gnu/autoconf/autoconf-2.71.tar.gz
fi

if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi

tar xaf autoconf-2.71.tar.gz
cd build
../autoconf-2.71/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf autoconf-2.71


