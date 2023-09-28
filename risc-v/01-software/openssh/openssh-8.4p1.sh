#!/bin/sh


set -e

mkdir -p build
packagename=openssh-8.4p1
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext


if [ ! -f openssh-8.4p1.tar.gz ]; then
	wget https://mirror.edgecast.com/pub/OpenBSD/OpenSSH/portable/openssh-8.4p1.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf openssh-8.4p1.tar.gz
cd build
mkdir -p /var/tempty
#groupadd sshd
useradd -g nobody -c 'sshd-privsep' -d /var/emtpy -s /bin/false sshd
../openssh-8.4p1/configure --prefix=/usr/local --sysconfdir=/etc/ssh --with-privsep-path=/var/empty --with-privsep-user=sshd --with-ssl-engine --with-md5-passwords
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf openssh-8.4p1

