#!/bin/bash

make
rm config_errors.log
cd invalid_config/
nb=$(ls | wc -l)
cd ..
i=0
while (( i < nb ))
do
	./webserv invalid_config/$i.json 2>> config_errors.log
	((i++))
done

cat config_errors.log
