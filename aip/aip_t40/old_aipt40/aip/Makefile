#aip_Makefile                                                                                                                                                                            

build:
	mkdir build
	cd build/ && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/uclibc.mips.toolchain.cmake -DBUILD_ENV=CSE -DCMAKE_INSTALL_PREFIX=../__release_uclibc_t40 -DNNA_DRIVER_PATH=/tmp/to_ATD/nna/ ..;make -j16


.PHONY: glibc
glibc:
	mkdir build
	cd build/ && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/glibc.mips.toolchain.cmake -DBUILD_ENV=CSE -DCMAKE_INSTALL_PREFIX=../__release_glibc_t40 -DNNA_DRIVER_PATH=/tmp/to_ATD/nna/ ..;make -j16


.PHONY: install
install:
	cd build/ && make install

.PHONY: clean
clean:
	rm __release_uclibc_t40/ __release_glibc_t40/ build/ tags -rf
	ctags -R --fields=+iaS --extra=+q

