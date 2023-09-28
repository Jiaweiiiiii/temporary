#!/bin/sh

set -e
mkdir -p build
packagename=flex-2.6.4
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f flex-2.6.4.tar.gz ];then
	wget https://github.com/westes/flex/files/981163/flex-2.6.4.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf flex-2.6.4.tar.gz
cd build
cp ../../config/config.* ../flex-2.6.4/build-aux/
../flex-2.6.4/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf flex-2.6.4
