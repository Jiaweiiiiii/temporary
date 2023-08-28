#/bin/sh
test_num=0
test_cnt=$1
while [ $test_num -lt $test_cnt ]
do
	test_num=$(( $test_num + 1 ))
	#./aipv20_convert.exe &
	#./aipv20_ddr3.exe &
	#./nndma.exe &
	./a.out &
done
echo run ${test_num} times
