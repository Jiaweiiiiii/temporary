#/bin/sh
test_num=0
test_cnt=$1
while [ $test_num -lt $test_cnt ]
do
	test_num=$(( $test_num + 1 ))
	#./aipv20_ddr.exe
	#./aipv20_oram.exe
	#./test_bin 
	#./test_bin_old
	./nnav20_dma_to_device.exe 1000 -19& 
	#./nndma.exe&
done
echo run ${test_num} times
