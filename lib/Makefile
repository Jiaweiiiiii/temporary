# aipv20_Makefile

build:
	mkdir build
	touch ./build/README
	echo "cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..">./build/README
	cat ./build/README
	cd build/ && cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..;make -j16


.PHONY: install
install:
	cd build/ && make install

.PHONY: clean
clean:
	rm __release/ build/ -rf

.PHONY: ctags
ctags:
	ctags -R --fields=+iaS --extra=+q
