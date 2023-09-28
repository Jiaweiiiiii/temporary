#!/bin/sh

set -e
mkdir -p build
packagename=git-2.17.1
packageext=tar.xz
packageurl=
../verify.sh $packagename $packageext

if [ ! -f git-2.17.1.tar.xz ];then
	wget https://mirrors.edge.kernel.org/pub/software/scm/git/git-2.17.1.tar.xz
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi 
tar xaf git-2.17.1.tar.xz
cd git-2.17.1
./configure --without-tcltk --with-curl --with-openssl --prefix=/
make NO_TCLTK=YesPlease all 2>&1 | tee make.all.out
make install
cd ..
rm -rf git-2.17.1

cd /etc
touch gitconfig

git config --system http.sslVerify false
git config --system pager.branch false

