rebuild:
rm 7.2.0 -rf

mkdir build
cd build
cmake -DMIPS_GLIBC_COMPILER=ON -DVENUS_MEM_MSG_ENABLE=OFF ../
xargs rm < install_manifest.txt
make -j4
make install

remake:
xargs rm < install_manifest.txt
make clean
make install
