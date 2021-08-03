#!/bin/bash
# cache-control: no-cache
# connection: keep-alive
# content-length: 1857
# content-type: multipart/form-data; boundary=--------------------------181455433616366442896447
# host: localhost:4000
# method: POST
# path: /test
# postman-token: 6eac2f23-2e4b-400e-94c7-e710efe54372
# protocol: HTTP/1.1
# user-agent: PostmanRuntime/7.28.2

export CONTENT_TYPE="multipart/form-data; boundary=--------------------------181455433616366442896447"
export SERVER_PROTOCOL=http/1.1
export CONTENT_LENGTH=1857
export REQUEST_METHOD=POST
export SCRIPT_FILENAME=./test.php
export SCRIPT_NAME=/test.php
export REDIRECT_STATUS=200

cat upload-form.txt | php-cgi