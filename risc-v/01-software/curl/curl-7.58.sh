#!/bin/sh

set -e
mkdir -p build
packagename=curl-7.58.0
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f curl-7.58.0.tar.xz ];then
	wget https://curl.se/download/curl-7.58.0.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf curl-7.58.0.tar.xz
cd build
../curl-7.58.0/configure --without-nss --prefix=/ --with-libssh2
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf curl-7.58.0
