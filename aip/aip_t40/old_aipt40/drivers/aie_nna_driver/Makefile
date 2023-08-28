#aip_Makefile                                                                                                                                                                            
.PHONY: uclibc
uclibc:
	mkdir build
	cd build/ && cmake -DCMAKE_TOOLCHAIN_FILE=../mips.toolchain.cmake -DCMAKE_INSTALL_PREFIX=/tmp/to_ATD ..;make -j16

.PHONY: glibc
glibc:
	mkdir build
	cd build/ && cmake -DCMAKE_TOOLCHAIN_FILE=../mips.toolchain2.cmake -DCMAKE_INSTALL_PREFIX=/tmp/to_ATD ..;make -j16


.PHONY: install
install:
	cd build/ && make install

.PHONY: clean
clean:
	rm /tmp/to_ATD/nna/ build/ tags -rf
	ctags -R --fields=+iaS --extra=+q

