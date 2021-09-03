
# ALL=$(ls srcs/)

# for script in ${ALL[*]}
# do
#     echo $script
# done

RED="\e[91m"
GREEN="\e[92m"
CYAN="\e[96m"
WHITE="\e[97m"

echo -en $CYAN
echo "     ***     WELCOME TO WEBSERV SCRIPT     ***
  This script is made to show you different behaviors of the webserv project
  No output is checked by this script, it's up to you to check the output,
  to ask the corrected team to explain the different behaviors,
  and judge if it's ok or not."

echo -e $GREEN
read -p '  press enter to continue, ctrl+C to stop' test
echo
echo
echo -en $GREEN
read -p 'About to launch curl --resolve tests, press enter to continue, ctrl+C to stop' test
echo -en $WHITE
bash srcs/curl_resolve.sh
echo
echo
echo -en $GREEN
read -p 'About to launch get tests, press enter to continue, ctrl+C to stop' test
echo -en $WHITE
bash srcs/get.sh
echo
echo
echo -en $GREEN
read -p 'About to launch post tests, press enter to continue, ctrl+C to stop' test
echo -en $WHITE
bash srcs/post.sh
echo
echo
echo -en $GREEN
read -p 'About to launch upload tests, press enter to continue, ctrl+C to stop' test
echo -en $WHITE
bash srcs/upload.sh
echo
echo
echo -en $GREEN
read -p 'About to launch download tests, press enter to continue, ctrl+C to stop' test
echo -en $WHITE
bash srcs/download.sh
echo
echo
echo -en $GREEN
read -p 'About to launch delete tests, press enter to continue, ctrl+C to stop' test
echo -en $WHITE
bash srcs/delete.sh
echo
echo
echo -en $GREEN
read -p 'About to launch siege tests, press enter to continue, ctrl+C to stop' test
echo -en $WHITE
bash srcs/siege.sh