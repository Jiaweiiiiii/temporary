#!/bin/sh


set -e

mkdir -p build
packagename=shadow-4.10
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f shadow-4.10.tar.xz ];then
	wget https://github.com/shadow-maint/shadow/releases/download/v4.10/shadow-4.10.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf shadow-4.10.tar.xz
cd build
../shadow-4.10/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf shadow-4.10


