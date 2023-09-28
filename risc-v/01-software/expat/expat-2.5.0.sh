#!/bin/sh

set -e

mkdir -p build
packagename=expat-2.5.0
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f expat-2.5.0.tar.xz ];then
	wget https://github.com/libexpat/libexpat/releases/download/R_2_5_0/expat-2.5.0.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf expat-2.5.0.tar.xz
cd build
../expat-2.5.0/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf expat-2.5.0
