#aip_Makefile                                                                                                                                                                            

build:
	mkdir build
	cd build/ && cmake -DMIPS_GLIBC_COMPILER=ON -DVENUS_MEM_MSG_ENABLE=OFF -DCHIP_NAME=T41 ../;make -j8

.PHONY: install
install:
	cd build/ && make install

.PHONY: clean
clean:
	rm 7.2.0/ build/ tags -rf
	ctags -R --fields=+iaS --extra=+q

