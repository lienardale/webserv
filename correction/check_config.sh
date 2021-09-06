#!/bin/bash

make -C ../
rm config_errors/config_errors*.log
cd invalid_config/
nb=$(ls | wc -l)
cd ..
cd ..
i=0
while (( i < nb ))
do
	echo "Checking invalid_config/$i.json"
	./webserv correction/invalid_config/$i.json 2>> correction/config_errors/config_errors$i.log
	cat correction/config_errors/config_errors$i.log
	echo
	((i++))
done

cat correction/config_errors/config_errors*.log > correction/config_errors/config_errors.log