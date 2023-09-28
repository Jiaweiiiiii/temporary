#!/bin/sh

set -e
mkdir -p build
packagename=automake-1.16.5
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f automake-1.16.5.tar.xz ];then
	wget https://ftp.gnu.org/gnu/automake/automake-1.16.5.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf automake-1.16.5.tar.xz
cd build
../automake-1.16.5/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf automake-1.16.5

