#!/bin/sh

set -e
mkdir -p build
packagename=perl-5.34.1
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f perl-5.34.1.tar.gz ];then 
	wget https://www.cpan.org/src/5.0/perl-5.34.1.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
if ! [ -x "$(command -v cc)" ]; then
	ln -s `which gcc` /bin/cc
fi
tar xaf perl-5.34.1.tar.gz
cd perl-5.34.1
./Configure -des
make test 2>&1 | tee make.test.out
make install
make clean

