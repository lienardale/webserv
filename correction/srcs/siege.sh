
RED="\e[91m"
GREEN="\e[92m"
CYAN="\e[96m"
WHITE="\e[97m"

echo -en $GREEN
echo -n "Installing siege.."
sudo apt update > /dev/null 2> /dev/null
sudo apt install siege > /dev/null 2> /dev/null
echo ".complete"
echo
echo -en $CYAN
echo " -> launching siege
        ---> use ctrl+C to stop when you think it's been long enough"
echo -e $WHITE
siege -b 127.0.0.1:8000