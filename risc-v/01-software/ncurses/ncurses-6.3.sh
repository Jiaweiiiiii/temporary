#!/bin/sh

set -e
mkdir -p build
packagename=ncurses-6.3
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f ncurses-6.3.tar.gz ]; then
	wget https://invisible-mirror.net/archives/ncurses/ncurses-6.3.tar.gz
fi

if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi

tar xaf ncurses-6.3.tar.gz
cd build
../ncurses-6.3/configure  --with-shared --disable-widec
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf ncurses-6.3

