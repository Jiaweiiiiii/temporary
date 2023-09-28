#!/bin/sh

set -e

mkdir -p build

packagename=cmake-3.25.2
packageext=tar.gz
packageurl=https://github.com/Kitware/CMake/releases/download/v3.25.2/cmake-3.25.2.tar.gz
../verify.sh $packagename $packageext

if [ ! -f $packagename.$packageext ];then
	wget $packageurl
fi

if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf $packagename.$packageext
cd build
../$packagename/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf $packagename

