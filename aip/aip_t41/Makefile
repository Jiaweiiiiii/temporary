#aip_Makefile                                                                                                                                                                            

build:
	mkdir build
	cd build/ && cmake ..;make

.PHONY: install
install:
	cd build/ && make install

.PHONY: clean
clean:
	rm __release_aip_t41 build/ tags -rf
	ctags -R --fields=+iaS --extra=+q

