i=0;
while [[ 1 ]]
do
	./tools/gen_random_bin
	let i=i+1
	if [ $i -gt 10000 ];
	then
		break
	fi
done

