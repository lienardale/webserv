#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <string>


int main (void){
    std::string str;
    int fd[2];
    int n;
    str = "coucou les zouzous";
    std::stringstream tmp;
    tmp << str;
    pipe(fd);
    dup2( fd[0], STDIN_FILENO );
    write(fd[1],str.c_str(), strlen(str.c_str()));
    // write(fd[1], );
    std::cin >> n;
    std::cout << n << std::endl;

    return 0;
}