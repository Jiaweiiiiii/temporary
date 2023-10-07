#!/bin/sh

set -e
export set JUST_DOWNLOAD=0

echo "Build & Install softwares"

dt=$(date)
if [[ "$dt" =~ "1970" ]];then
	date -s "2022-12-09"
fi

rm -rf */build

cd ./tar
./tar-1.34.sh
cd ..

cd ./coreutils
./coreutils-8.28.sh
cd ..

cd ./expat
./expat-2.5.0.sh
cd ..

cd ./findutils
./findutils-4.7.0.sh
cd ..

cd ./grep
./grep-3.1.sh
cd ..

cd m4
./m4-1.4.19.sh
cd ..

cd ./libtool
./libtool-2.4.7.sh
cd ..

cd ./shadow
./shadow-4.10.sh
cd ..

cd ./zlib
./zlib-1.2.13.sh
cd ..

cd ./texinfo
./texinfo-6.5.sh
cd ..

cd openssl
./openssl-1.1.1s.sh
cd ..

cd openssh
./openssh-8.4p1.sh
cd ..

cd awk
./gawk-5.0.0.sh
cd ..

cd ncurses
./ncurses-6.3.sh
cd ..

cd bash
./bash-5.1.16.sh
cd ..

cd libunistring
./libunistring-1.1.sh
cd ..

cd libidn2
./libidn2-2.3.4.sh
cd ..

cd libssh2
./libssh2-1.10.0.sh
cd ..

cd curl
./curl-7.58.sh
cd ..

cd libiconv
./libiconv-1.17.sh
cd ..

cd pkg-config
./pkg-config-0.29.2.sh
cd ..

cd sudo
./sudo-1.9.12.sh
cd ..

cd flex
./flex-2.6.4.sh
cd ..

cd bison
./bison-3.8.2.sh
cd ..

cd less
./less-608.sh
cd ..

cd autoconf
./autoconf-2.71.sh
cd ..

cd automake
./automake-1.16.5.sh
cd ..

cd gettext
./gettext-0.20.2.sh
cd ..

cd git
./git-2.17.1.sh
cd ..

cd ./procps-ng
./procps-ng-3.3.12.sh
cd ..

cd vim
./vim-8.0.sh
cd ..

cd perl
./perl-5.34.1.sh
cd ..

cd cmake
./cmake-3.25.2.sh
cd ..

sed -i 1croot:x:0:0:root:/root:/bin/bash /etc/passwd

echo "nutles os : all software install finished !!!\n"

