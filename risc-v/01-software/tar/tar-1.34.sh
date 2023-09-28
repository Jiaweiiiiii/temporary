#!/bin/sh


set -e

mkdir -p build
packagename=tar-1.34
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f tar-1.34.tar.xz ];then 
	wget https://ftp.gnu.org/gnu/tar/tar-1.34.tar.xz
fi

#export set JUST_DOWNLOAD=1
if [ $JUST_DOWNLOAD -eq 1 ];then
	tar xaf tar-1.34.tar.xz
	tar cf tar-1.34.tar tar-1.34
	rm -rf tar-1.34
	exit 0
fi

tar xf tar-1.34.tar
mv `which tar` /bin/tar-busybox
cd build
../tar-1.34/configure FORCE_UNSAFE_CONFIGURE=1
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf tar-1.34

