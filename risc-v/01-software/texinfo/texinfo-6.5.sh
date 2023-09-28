#!/bin/sh


set -e

mkdir -p build
packagename=texinfo-6.5
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f texinfo-6.5.tar.xz ];then
	wget https://ftp.gnu.org/gnu/texinfo/texinfo-6.5.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf texinfo-6.5.tar.xz
cp ../config/config.* texinfo-6.5/build-aux/
cd build
../texinfo-6.5/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf texinfo-6.5

