#!/bin/sh

set -e
mkdir -p build
packagename=pkg-config-0.29.2
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f pkg-config-0.29.2.tar.gz ];then
	wget https://pkgconfig.freedesktop.org/releases/pkg-config-0.29.2.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf pkg-config-0.29.2.tar.gz
cd build
cp ../../config/config.* ../pkg-config-0.29.2/
cp ../../config/config.* ../pkg-config-0.29.2/glib/
../pkg-config-0.29.2/configure --with-internal-glib --with-libiconv=gnu
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf pkg-config-0.29.2

