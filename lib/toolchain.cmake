set (CMAKE_SYSTEM_NAME Linux)
set (CMAKE_SYSTEM_PROCESSOR Mips)

#set (TOOLCHAIN_PATH /disk0/tools/ingenic/mips-gcc520-glibc222/bin/)
set (TOOLCHAIN_PATH "")

set (CMAKE_C_COMPILER ${TOOLCHAIN_PATH}mips-linux-gnu-gcc)
set (CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}mips-linux-gnu-g++)

set (CMAKE_AR      "${TOOLCHAIN_PATH}mips-linux-gnu-ar" CACHE FILEPATH "" FORCE)
set (CMAKE_NM      "${TOOLCHAIN_PATH}mips-linux-gnu-nm")
set (CMAKE_LINKER  "${TOOLCHAIN_PATH}mips-linux-gnu-gcc")
set (CMAKE_OBJDUMP "${TOOLCHAIN_PATH}mips-linux-gnu-objdump")
set (CMAKE_RANDLIB "${TOOLCHAIN_PATH}mips-linux-gnu-randlib")
