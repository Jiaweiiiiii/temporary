#/bin/sh

packagename=$1
packageext=$2
#echo $1:$2:$3
if [ -f $packagename.$packageext ] && [ $JUST_DOWNLOAD -eq 1 ];then
        md5code=$(md5sum $packagename.$packageext)
        n=0
        for stritem in $md5code;do
                n=`expr $n + 1`
                eval md5code$n="$stritem"
        done
        md5line=$(cat ../verify.md5 | grep -n ${md5code1} | awk -F ":" '{print $1}')
        packagenameline=$(cat ../verify.md5 | grep -n ${md5code2} | awk -F ":" '{print $1}')
	#echo "md5line:$md5line"
	#echo "packagenameline:$packagenameline"
        if [ "$md5line" != "$packagenameline" ];then
                rm $packagename.$packageext
		echo "file md5sum verify not correct !"
		#echo "1"
		exit 1
        else
                #echo "$packagename.$packageext md5 verify ok"
		#echo "0"
		exit 0
        fi
elif [ $JUST_DOWNLOAD -eq 0 ];then
	echo "no more need download"
else
	echo $1.$2
	echo "something wrong"
fi
