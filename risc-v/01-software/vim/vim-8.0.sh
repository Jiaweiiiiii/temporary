#!/bin/sh

set -e
mkdir -p build
packagename=vim-8.0
packageext=tar.bz2
packageurl=
../verify.sh $packagename $packageext


if [ ! -f vim-8.0.tar.bz2 ];then
	wget ftp://ftp.vim.org/pub/vim/unix/vim-8.0.tar.bz2
fi
if [ $JUST_DOWNLOAD -eq 1 ];then
	exit 0
fi 
tar xaf vim-8.0.tar.bz2
cd vim80
./configure
make 2>&1 | tee make.out
make install
make clean

touch ~/.bashrc
echo 'export TERM=xterm-256color' > ~/.bashrc
source ~/.bashrc

