#!/bin/sh

set -e 
mkdir -p build
packagename=ntp-dev-4.3.99
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f ntp-dev-4.3.99.tar.gz ];then
	wget http://www.eecis.udel.edu/~ntp/ntp_spool/ntp4/ntp-dev/ntp-dev-4.3.99.tar.gz
fi

if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
#tar xaf ntp-dev-4.3.99.tar.gz
#cd build
#../ntp-dev-4.3.99/configure
