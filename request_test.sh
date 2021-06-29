
# docker build -t nginx_test_serv nginx/.
# docker run --name nginx_test -p 5000:80 nginx_test_serv &

# service nginx restart

make run &

{ echo "GET caca.html HTTP/1.1 
Host: localhost:80" } | telnet localhost 80 > nginx.log


echo "GET caca.html HTTP/1.1 
Host: localhost:8000" | telnet localhost 8000 > us.log
# GET caca.html HTTP/1.1
# Host: localhost:8000

# docker kill nginx_test
# docker rm nginx_test
kill $(ps aux | pgrep webserv | grep -v grep)
