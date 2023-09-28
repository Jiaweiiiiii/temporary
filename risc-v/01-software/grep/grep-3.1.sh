#!/bin/sh


set -e

mkdir -p build
packagename=grep-3.1
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f grep-3.1.tar.xz ];then	
	wget https://ftp.gnu.org/gnu/grep/grep-3.1.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf grep-3.1.tar.xz
cd build
../grep-3.1/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf grep-3.1
mv `which grep` /bin/grep-busybox
