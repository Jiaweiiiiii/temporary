T41 AIP farmework testsuite
=====================================

The 'model/' is used to store various test images and data.
Other testing programs with aipv20 directory.                                                                                                                                                

Operating Steps
1. Enter the testing function directory.
	e.g. : cd aipv20_ddr

2. Check Makefile.
	e.g. : vim Makefile
	e.g. : CC = mips-linux-gnu-gcc
	e.g. : CFLAGS = -Wall -I../../__release0710_aipv20/include
	e.g. : LIBDIR = ../../__release0710_aipv20/lib/uclibc -muclibc

3. Check the content of the image path in the code.
	e.g. : vim aipv20_resize.c
	e.g. : "./model/day_w1024_h576.nv12"
	

4. Create an exe file.
	e.g. : make clean;make -j16

5. Move the test program and the images that need to be
processed to the running directory.
	e.g. : cp aipv20_ddr.exe /home/nfsroot/xxx

