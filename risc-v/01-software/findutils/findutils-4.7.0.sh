#!/bin/sh


set -e

mkdir -p build
packagename=findutils-4.7.0
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f findutils-4.7.0.tar.xz ];then
	wget https://ftp.gnu.org/gnu/findutils/findutils-4.7.0.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf findutils-4.7.0.tar.xz
cd build
../findutils-4.7.0/configure
make 2>&1 | tee make.out
make install
mv /usr/bin/find /usr/bin/find-busybox
cd ..
rm -rf build
rm -rf findutils-4.7.0
