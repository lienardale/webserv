read -p 'about to launch curl --resolve tests, press enter to continue, ctrl+C to stop' test
bash srcs/curl_resolve.sh
echo
echo
read -p 'about to launch get tests, press enter to continue, ctrl+C to stop' test
bash srcs/get.sh
echo
echo
read -p 'about to launch post tests, press enter to continue, ctrl+C to stop' test
bash srcs/post.sh
echo
echo
read -p 'about to launch upload tests, press enter to continue, ctrl+C to stop' test
bash srcs/upload.sh
echo
echo
read -p 'about to launch download tests, press enter to continue, ctrl+C to stop' test
bash srcs/download.sh
echo
echo
read -p 'about to launch delete tests, press enter to continue, ctrl+C to stop' test
bash srcs/delete.sh
echo
echo
read -p 'about to launch siege tests, press enter to continue, ctrl+C to stop' test
bash srcs/siege.sh