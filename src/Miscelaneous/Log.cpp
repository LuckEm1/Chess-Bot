#include "..\header\Game_Header.h"
#include <iostream>

/*		  LOG 		 */
//----------------------------------------------------------
void Log::Info(std::string message) { std::cout << "[Info]: " << message << std::endl; }
void Log::Int(int message) { std::cout << "[Info]: " << its(message) << std::endl; }
void Log::Error(std::string message) { std::cout << "[ERROR]: " << message << std::endl; }
void Log::Warning(std::string message) { std::cout << "[WARNING]: " << message << std::endl; }
void Log::Print(std::string message) { std::cout << message << std::endl; }
void Log::Char(char message) { std::cout << message << std::endl; }
//----------------------------------------------------------


/*        iostream external access        */
//----------------------------------------------------------
std::string its(int variable) { return std::to_string(variable); }
std::string str_cin(std::string message)
{
	if (message != "")
	{
		Log log;
		log.Print(message);
	}
	std::string info;
	std::cin >> info;
	return info;
}
int int_cin(std::string message)
{
	if (message != "")
	{
		Log log;
		log.Print(message);
	}
	int info;
	std::cin >> info;
	return info;
}
//----------------------------------------------------------
