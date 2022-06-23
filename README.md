# webserv
a HTTP server in C++ 98



<a href="https://github.com/JaeSeoKim/badge42"><img src="https://badge42.vercel.app/api/v2/cl4cq9w7c004409k3asnbj82v/project/2119669" alt="alienard's 42 webserv Score" /></a>

## demo :
We tailored some scripts that demonstrated our server worked as intended
<p align="left">
	<img src="./assets/webserv_demo.gif" >
</p>

## team :
this is a project form 42 Paris school's comon-core
it was coded by a team of three students : 
- [Pierre Cariou](https://github.com/pierrecariou) 
- [Damien Boyer](https://github.com/BoyerDamien) 
- [Alexandre Lienard](https://github.com/lienardale)

it was the first project where were more than two people, which obligated us to discover some good practices :
- git branches
- pull requests
- git actions
- minimum code review number before merging pull request 

## code stats :

- 3 contributors
- Total 6604 lines
  - C++ : 3813 lines, 57.7%
  - json (invalid & valids configs) : 982 lines, 14.9%
  - Shell : 227 lines, 3.4%
  - Other (html, css, php...) : 0,4%
- Git
  - 113 branches
  - 104 pull requests
  - 405 commits

## languages and tools:
<p align="left"> 
For the correction scripts : <a href="https://www.gnu.org/software/bash/" target="_blank" rel="noreferrer"> 
    <img src="https://www.vectorlogo.zone/logos/gnu_bash/gnu_bash-icon.svg" alt="bash" width="40" height="40"/> 
</a>

To code the server : <a href="https://www.w3schools.com/cpp/" target="_blank" rel="noreferrer"> 
    <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" alt="cplusplus" width="40" height="40"/> 
</a> 

For the asynchronous team-work, pull-requests & git actions : <a href="https://git-scm.com/" target="_blank" rel="noreferrer"> 
    <img src="https://www.vectorlogo.zone/logos/git-scm/git-scm-icon.svg" alt="git" width="40" height="40"/> 
</a>

To replicate the behavior of a production-grade server : <a href="https://www.nginx.com" target="_blank" rel="noreferrer"> 
    <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/nginx/nginx-original.svg" alt="nginx" width="40" height="40"/> 
</a> 

To test our requests along the way : <a href="https://postman.com" target="_blank" rel="noreferrer"> 
    <img src="https://www.vectorlogo.zone/logos/getpostman/getpostman-icon.svg" alt="postman" width="40" height="40"/> 
</a> 
</p>

## features :
- ### HTTP Requests
    * GET
    * POST
    * DELETE
- ### Configuration
    * location
    * server_name
    * ports
    * root / path
    * autoindex
- ### File Management
    * upload
    * download
    * mime_type
- ### CGI
    * php_cgi binary (php's cgi binary)
    * print_cgi (custom cgi binary for testing)
- ### error handling
    * max_bodysize
    * headers
    * sockets
    * custom error pages

## run it :
- make sure the localhost ports you intend to use ard free
- "git clone && cd webserv"
- "make"
- "./webserv &" (add your own config file as a 2nd arg if you want, otherwise it will take the default one
- "cd correction && bash test.sh" (and explore the tests we prepared)
- go to http://localhost:8000/ in the navigator of your choice (to see a magnificent static website perfectly rendered)
- don't forget to kill the process once you've finished ("kill $(ps | grep webserv | cut -d " " -f 1)"
)

## documentation :

- Basics of an HTTP Server
    - Client / Server (software)
        - Request / Response
            - I/O Multiplexing / FD handling (sockets-> FD which are connected)
            - Header / Body
            - accepted or not regarding the Configuration File
            - CGI (if necessary)

- EPOLL :
    - how it works : multiplexing fct, monitors activities on several fd (non-blocking)
            only in Linux, data_struct, direct access (constant) to fds. So it is much more efficient.
            e-poll -> event-based poll, we listen to events, and then iterate on then (events are read/write/error).
    - why we use it (rather than select or poll) : select and poll controll fd with a linear complexity algorithm
            (do a while on every fd). They are very compatible through systems.
    - How to accept client requests
    - How we send responses to client


- Subject requirements :
    - "only one read or one write per client per epoll" 
        -----> server.cpp - line 140
    - If error when error during a read/recv/write/send on a socket, client is removed
    - checking only if ret == 0 && ret == -1
        -----> socket.cpp - line 208 & line 224
        -----> remove client in catch in server.cpp - line 236
    - no check of errno
    - Multiple servers at the same time with different config but common ports
- Invalidity reasons of configs
    - Config file 1 : multiple servers with different port -> ok
    - Config file 2 : multiple servers with different hostname (curl -resolve to test) -> ok
    - Config file 3 : setup default error page -> ok
    - Config file 4 : client max body size (test with body > / = / <) -> ok
    - Config file 5 : routes in a server to different directories -> ok
    - Config file 6 : default file to search if asked for a directory -> ok
    - Config file 7 : accepted methods for a route (delete file with and without permission) -> ok
    - Config file 8 : multiple ports and different websites -> ok
    - Config file 9 : same port multiple times (works if different server_name just like nginx) -> ok

- How does the correction scripts test our server : 
    - telnet / curl scripts
    -> GET
    -> POST
    -> DELETE
    -> OTHER REQUESTS (should not produce any crash)
    - upload / download script

- To test Further : Browser tests (Firefox)
    - Fully static website
    - wrong url
    - list a directory
    - redirected url

- Siege
    - availability > 99.5% for a get on an empty page (siege -b)
    - use it indefinetely without restarting the server

- Memory leaks : valgring

- Hanging connection -> re-up ok (which means that sockets are closed) 