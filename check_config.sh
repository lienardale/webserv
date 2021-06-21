#!/bin/bash

make
rm config_errors/config_errors*.log
cd invalid_config/
nb=$(ls | wc -l)
cd ..
i=0
while (( i < nb ))
do
	echo "Checking invalid_config/$i.json"
	./webserv invalid_config/$i.json 2>> config_errors/config_errors$i.log
	cat config_errors/config_errors$i.log
	echo
	((i++))
done

cat config_errors/config_errors*.log > config_errors/config_errors.log