echo "Files in $pwd before dld :"
echo -n "--> "
pwd
ls
echo

curl localhost:8000/uploads/upload_test_1.c --output upload_test_1.c
echo
echo "Files in $pwd after dld :"
echo -n "--> "
pwd
ls
rm upload_test_1.c