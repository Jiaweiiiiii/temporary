#!/bin/sh

set -e
mkdir -p build
packagename=libssh2-1.10.0
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f libssh2-1.10.0.tar.gz ]; then
	wget https://www.libssh2.org/download/libssh2-1.10.0.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf libssh2-1.10.0.tar.gz
cd build
../libssh2-1.10.0/configure
make 2>&1 | tee make.out
make install
ldconfig
cd ..
rm -rf build
rm -rf libssh2-1.10.0

