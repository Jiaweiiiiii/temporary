#!/bin/sh


set -e

mkdir -p build
packagename=bash-5.1.16
packageext=tar.gz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f bash-5.1.16.tar.gz ]; then
	wget https://ftp.gnu.org/gnu/bash/bash-5.1.16.tar.gz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi
tar xaf bash-5.1.16.tar.gz
cd build
../bash-5.1.16/configure --prefix=/
make 2>&1 | tee make.out
make install
cd ..
rm -rf build
rm -rf bash-5.1.16
echo '/bin/bash' > /etc/shells
#sed -i 's#root:x:0:0:root:/root:*#&/bin/bash:#g' /etc/passwd
mkdir -p /var/spool/mail
cp -r ./skel /etc/
