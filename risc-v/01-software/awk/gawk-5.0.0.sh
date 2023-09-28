#!/bin/sh

set -e

mkdir -p build
packagename=gawk-5.0.0
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f gawk-5.0.0.tar.xz ];then
	wget http://ftp.gnu.org/gnu/gawk/gawk-5.0.0.tar.xz
fi

if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf gawk-5.0.0.tar.xz
cd build
../gawk-5.0.0/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf gawk-5.0.0
mv /usr/bin/awk /usr/bin/awk-bak

