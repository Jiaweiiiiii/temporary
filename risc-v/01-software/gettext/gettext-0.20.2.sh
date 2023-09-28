#!/bin/sh

set -e
mkdir -p build
packagename=gettext-0.20.2
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f gettext-0.20.2.tar.gz ];then
	wget https://ftp.gnu.org/pub/gnu/gettext/gettext-0.20.2.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf gettext-0.20.2.tar.gz
cd build
../gettext-0.20.2/configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf gettext-0.20.2

ldconfig

