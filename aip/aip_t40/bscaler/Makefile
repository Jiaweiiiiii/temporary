#aip_Makefile                                                                                                                                                                            

build:
	mkdir build1
	mkdir build2
	cd build1/ && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/uclibc.mips.toolchain.cmake -DBUILD_ENV=CSE -DCMAKE_INSTALL_PREFIX=../__release_uclibc_t40 -DNNA_DRIVER_PATH=/tmp/to_ATD/nna/ ..;make -j16 && make install
	cd ../
	cd build2/ && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/glibc.mips.toolchain.cmake -DBUILD_ENV=CSE -DCMAKE_INSTALL_PREFIX=../__release_glibc_t40 -DNNA_DRIVER_PATH=/tmp/to_ATD/nna/ ..;make -j16 && make install
	rm build1/ build2/ -r



.PHONY: uclibc
uclibc:
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
	rm __release_uclibc_t40/ __release_glibc_t40/ build/ build1/ build2/ tags -rf
	ctags -R --fields=+iaS --extra=+q

