read -p 'about to launch get tests, press enter to continue, ctrl+C to stop' test
bash get.sh
echo
echo
read -p 'about to launch post tests, press enter to continue, ctrl+C to stop' test
bash post.sh
echo
echo
read -p 'about to launch upload tests, press enter to continue, ctrl+C to stop' test
bash upload.sh
echo
echo
read -p 'about to launch download tests, press enter to continue, ctrl+C to stop' test
bash download.sh
echo
echo
read -p 'about to launch delete tests, press enter to continue, ctrl+C to stop' test
bash delete.sh
echo
echo
read -p 'about to launch siege tests, press enter to continue, ctrl+C to stop' test
bash siege.sh