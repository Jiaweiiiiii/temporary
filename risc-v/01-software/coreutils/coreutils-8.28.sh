#!/bin/sh

set -e

mkdir -p build
packagename=coreutils-8.28
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f coreutils-8.28.tar.xz ];then
	wget https://ftp.gnu.org/gnu/coreutils/coreutils-8.28.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf coreutils-8.28.tar.xz
cd coreutils-8.28
sed -i 's/IO_ftrylockfile/IO_EOF_SEEN/' lib/*.c
echo "#define _IO_IN_BACKUP 0x100" >> lib/stdio-impl.h
cd ..
cd build
../coreutils-8.28/configure FORCE_UNSAFE_CONFIGURE=1
make 2>&1 | tee make.log
make install
cd ..
rm -rf build
rm -rf coreutils-8.28
