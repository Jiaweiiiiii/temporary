##
##        (C) COPYRIGHT Ingenic Limited.
##             ALL RIGHTS RESERVED
##
## File       : README
## Authors    : jmqi@taurus
## Create Time: 2020-06-05:11:36:09
## Description:
##

1) x86 compile
   a) only compile C model
      cmake -DBUILD_ENV=X86 ..

   b) compile eyer
      cmake -DBUILD_ENV=EYER ..


mips编译步骤如下：
准备工作：如果没有build目录，请先创建build目录；
mkdir build

a)glibc
第一步：需要编译nna 相关的库，如果没有编译，请进入aie_nna_driver目录按照README的步骤进行编译。
第二步：修改cmake/glibc.mips.toolchain.cmake中的编译工具路径。例如：
		set(CMAKE_FIND_ROOT_PATH "/home_c/xhshen/work/isvp/prebuilt/toolchain/runtime/gcc_720/mips-gcc720-glibc226")
第三步：进入build目录执行cmake命令，各参数意义说明如下：
	  cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/glibc.mips.toolchain.cmake -DBUILD_ENV=CSE \
	  		-DCMAKE_INSTALL_PREFIX=../_install_glibc \
			-DNNA_DRIVER_PATH=第一步编译出下nna库的绝对路径 ..
		
第四步：make -j16;make install
第五步：查看_install_glibc目录下生成的文件

a)uclibc
第一步：需要编译nna 相关的库，如果没有编译，请进入aie_nna_driver目录按照README的步骤进行编译。
第二步：修改cmake/uclibc.mips.toolchain.cmake中的编译工具路径。例如：
		set(CMAKE_FIND_ROOT_PATH "/home_c/xhshen/work/isvp/prebuilt/toolchain/runtime/gcc_720/mips-gcc720-glibc226")
第三步：进入build目录执行cmake命令，各参数意义说明如下：
	  cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/uclibc.mips.toolchain.cmake -DBUILD_ENV=CSE \
	  		-DCMAKE_INSTALL_PREFIX=../_install_uclibc \
			-DNNA_DRIVER_PATH=/tmp/to_ATD/nna/ ..


 cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/uclibc.mips.toolchain.cmake -DBUILD_ENV=CSE -DCMAKE_INSTALL_PREFIX=../_install_uclibc -DNNA_DRIVER_PATH=/tmp/to_ATD/nna/ ..


第四步：make -j16;make install
第五步：查看_install_uclibc目录下生成的文件
