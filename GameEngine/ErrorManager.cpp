#include "ErrorManager.h"

#include <cstdlib>

void ErrorManager::error(std::string eMessage, ErrorLevel eLevel, bool log) 
{
	std::string readableELevel;
	switch (eLevel) {
	case ErrorLevel::LOW:
		readableELevel = "low";
		break;
	case ErrorLevel::MEDIUM:
		readableELevel = "medium";
		break;
	case ErrorLevel::HIGH:
		readableELevel = "high";
		break;
	case ErrorLevel::FATAL:
		readableELevel = "FATAL";
		break;
	}
	std::cout << "[" << readableELevel << "] : " << eMessage << std::endl;

	if (log) {
		ErrorManager::logError(eMessage, readableELevel);
	}

	if (eLevel == ErrorLevel::FATAL) {
		SDL_Quit();
		exit(-5);
	}

	return;
}

void ErrorManager::logError(std::string eMessage, std::string readableELevel)
{
	std::ofstream eLog;
	eLog.open("ErrorLog.txt", std::ios_base::app);
	eLog << "[" << readableELevel << "] : " << eMessage << "\n";
	eLog.close();
}