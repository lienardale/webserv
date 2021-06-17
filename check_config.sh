#!/bin/bash

make
rm config_errors.log
cd invalid_config/
nb=$(ls | wc -l)
i=0
while (( i < nb ))
do
	./../webserv $i.json 2>> ../config_errors.log
	((i++))
done