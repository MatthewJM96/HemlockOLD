#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
enum class ErrorLevel {
	LOW,
	MEDIUM,
	HIGH,
	FATAL
};
class ErrorManager
{
public:
	static void error(std::string eMessage, ErrorLevel eLevel, bool log);
private:
	static void logError(std::string eMessage, std::string readableELevel);
};

