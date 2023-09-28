#!/bin/sh

set -e
mkdir -p build
packagename=less-608
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f less-608.tar.gz ];then
	wget http://www.greenwoodsoftware.com/less/less-608.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf less-608.tar.gz
cd build
../less-608/configure --prefix=/
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf less-608

