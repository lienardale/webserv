echo "Files in /www/uploads before delete without method accepted :"
echo "--> "
ls ../www/uploads
echo
curl localhost:7000/uploads/upload_test_2.txt -X DELETE
echo "Files in /www/uploads after delete without method accepted :"
echo "--> "
ls ../www/uploads
echo

echo "Files in /www/uploads before delete with method accepted :"
echo "--> "
ls ../www/uploads
echo
curl localhost:8000/uploads/upload_test_2.txt -X DELETE
echo "Files in /www/uploads after delete with method accepted :"
echo "--> "
ls ../www/uploads
echo