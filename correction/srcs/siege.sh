echo "Installing siege"
sudo apt update
sudo apt install siege
echo
echo " -> launching siege
        ---> use ctrl+C to stop when you think it's been long enough"
siege -b 127.0.0.1:8000