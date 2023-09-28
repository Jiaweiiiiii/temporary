#!/bin/sh

set -e
mkdir -p build
packagename=bison-3.8.2
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f bison-3.8.2.tar.xz ]; then
	wget https://ftp.gnu.org/gnu/bison/bison-3.8.2.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf bison-3.8.2.tar.xz
cd build
../bison-3.8.2/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf bison-3.8.2

