#!/bin/bash
pclr.sh
sleep 5
for ((i=1;i<1000;i++))
do	
	./gen_c_vector
	result=$?
	echo $result
	if [[ $result == 0 ]]; then
		echo "frmc cfg success!"
		make -j32
		pclr.sh
		sleep 5
		cd my_t.vmem/
		. ncsim_cmd 0 &
		cd -
		./t_bscaler_c
	else
		echo "frmc cfg failed!"
		sleep 1
	fi
	echo "case num: $i"
done

