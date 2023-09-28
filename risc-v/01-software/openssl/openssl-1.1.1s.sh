#!/bin/sh


set -e

mkdir -p build
packagename=openssl-1.1.1s
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f openssl-1.1.1s.tar.gz ];then
	wget https://www.openssl.org/source/openssl-1.1.1s.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf openssl-1.1.1s.tar.gz
cd build
../openssl-1.1.1s/config shared zlib disable-afalgeng
make 2>&1 | tee make.out
make install
echo '/usr/local/lib' > /etc/ld.so.conf
ldconfig
cd ..
rm -rf build
rm -rf openssl-1.1.1s
openssl version


