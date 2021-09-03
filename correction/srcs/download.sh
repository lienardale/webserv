echo "Files in $pwd before dld :"
echo -n "--> "
pwd
ls
echo

curl -X GET -H "connection: keep-alive" -H "accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8" -H "accept-encoding: gzip, deflate" -H "accept-language: en-US,en;q=0.5" -H "sec-fetch-dest: document" -H "sec-fetch-mode: navigate" -H "sec-fetch-site: none" -H "sec-fetch-user: ?1" -H "upgrade-insecure-requests: 1" -H "user-agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:91.0) Gecko/20100101 Firefox/91.0" 127.0.0.1:8000/uploads/upload_test_1.c 
echo
echo "Files in $pwd after dld :"
echo -n "--> "
pwd
ls
rm upload_test_1.c