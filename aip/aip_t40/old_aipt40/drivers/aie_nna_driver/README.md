# Usage

## 1. build compile and install
```
	mkdir build; cd build;
	cmake -DCMAKE_TOOLCHAIN_FILE=../mips.toolchain.cmake -DCMAKE_INSTALL_PREFIX=/tmp/to_ATD ..
	make -j4
	make install
```
