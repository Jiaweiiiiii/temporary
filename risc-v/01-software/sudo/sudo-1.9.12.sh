#!/bin/sh

set -e 
mkdir -p build
packagename=sudo-1.9.12
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f sudo-1.9.12.tar.gz ]; then
	wget https://www.sudo.ws/dist/sudo-1.9.12.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi 
tar xaf sudo-1.9.12.tar.gz
cd build
../sudo-1.9.12/configure --with-timeout=10 --without-lecture --disable-root-sudo --disable-path-info --sysconfdir=/etc/ --bindir=/bin --sbindir=/sbin
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf sudo-1.9.12
sed -i '/# %sudo/s/^#//' /etc/sudoers

