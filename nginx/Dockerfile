FROM ubuntu:latest

COPY srcs/* srcs/

RUN apt update \
	&& apt install -y nginx

# Setup nginx
RUN mkdir -p /run/nginx www && \
	cp srcs/nginx.conf /etc/nginx/conf.d/default.conf && \
	# mv srcs/nginx.conf /etc/nginx/ && \
	mv srcs/index.html /www/ && \
    mv srcs/run.sh . && \
    chmod 755 run.sh

EXPOSE 80 443

CMD ./run.sh
