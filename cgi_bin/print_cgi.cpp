#include <sstream>
#include <iostream>

int main(int ac, char **av, char **env)
{
	std::cout << "\nWELCOME TO PRINT EVERY ARG_CGI\n" << std::endl;
  
  for (std::string line ; getline(std::cin, line); ){
      std::cout << "stdin:|" << line << "|" << std::endl;
  }
  std::cout << std::endl;
	if (ac && av)
	{
		std::cout << ac << " arguments were sent :" << std::endl;
		std::cout << std::endl;
		for (int i = 0 ; av[i]; i++)
			std::cout << "arg[" << i << "]:|" << av[i] << "|" << std::endl;
		std::cout << std::endl;
	}
	else{
		std::cout << "No arguments were sent." << std::endl;
	}
	if (env)
	{
		std::cout << "An environment was sent :" << std::endl;
		std::cout << std::endl;
		for (int i = 0 ; env[i]; i++)
			std::cout << "env[" << i << "]:|" << env[i] << "|" << std::endl;
		std::cout << std::endl;
	}
	else{
		std::cout << "No environment was sent." << std::endl;
	}
	return 0;
}
