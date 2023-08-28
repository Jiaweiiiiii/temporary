#!/bin/bash
# 
#         (C) COPYRIGHT Ingenic Limited
#              ALL RIGHT RESERVED
# 
# File        : merge_pmon_data.sh
# Authors     : klyu
# Create Time : 2020-12-22 09:41:46 (CST)
# Description :
#

if [ $# != 2 ]
then
	echo 'input is NULL'
	echo "Usage: $0 index.txt output.txt"
	echo "eg: $0 yolov3_layer_index.txt"\
		 "yolov3_pmon_output.txt"
	exit
else
	INDEX_FILE_NAME=$1
	OUTPUT_FILE_PATH=$2
fi

echo "merging pmon output data..."

while read LINE
do
	echo $LINE
	head -5 $LINE >> $OUTPUT_FILE_PATH
done < $INDEX_FILE_NAME

echo "finished."

