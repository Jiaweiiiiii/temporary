#!/bin/sh

set -e
mkdir -p build
packagename=diffutils-3.7
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f diffutils-3.7.tar.xz ];then
	wget https://ftp.gnu.org/gnu/diffutils/diffutils-3.7.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi 
tar xaf diffutils-3.7.tar.xz
cd diffutils-3.7
./configure
make 2>&1 | tee make.out
make install
cd ..
rm -rf diffutils-3.7
mv /usr/bin/diff /usr/bin/diff-busybox

