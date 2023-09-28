#!/bin/sh


set -e
mkdir -p build
packagename=procps-ng-3.3.12
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f procps-ng-3.3.12.tar.xz ];then
	wget https://nchc.dl.sourceforge.net/project/procps-ng/Production/procps-ng-3.3.12.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf procps-ng-3.3.12.tar.xz
cd build
cp ../../config/config.* ../procps-ng-3.3.12/
../procps-ng-3.3.12/configure --exec-prefix= --disable-static --disable-skill
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf procps-ng-3.3.12

