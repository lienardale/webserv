# webserv
a HTTP server in C++ 98



<a href="https://github.com/JaeSeoKim/badge42"><img src="https://badge42.vercel.app/api/v2/cl4cq9w7c004409k3asnbj82v/project/2119669" alt="alienard's 42 webserv Score" /></a>

## demo
We tailored some scripts that demonstrated our server worked as intended
<p align="left">
	<img src="./assets/webserv_demo.gif" >
</p>

## context
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

## Languages and Tools:
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

## features
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

## run it
- make sure the localhost ports you intend to use ard free
- "git clone && cd webserv"
- "make"
- "./webserv &" (add your own config file as a 2nd arg if you want, otherwise it will take the default one
- "cd correction && bash test.sh" (and explore the tests we prepared)
- go to http://localhost:8000/ in the navigator of your choice (to see a magnificent static website perfectly rendered)
- don't forget to kill the process once you've finished ("kill $(ps | grep webserv | cut -d " " -f 1)"
)