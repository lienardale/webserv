echo "Files in $PWD before dld :"
echo -n "--> "
ls
echo
curl 127.0.0.1:8000/uploads/upload_test_1.c -o upload_test_1.c -s
echo
echo "Files in $PWD after dld :"
echo -n "--> "
ls
cat upload_test_1.c
rm upload_test_1.c