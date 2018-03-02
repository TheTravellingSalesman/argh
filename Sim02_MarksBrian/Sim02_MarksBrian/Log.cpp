#include "Log.h"

Log::Log(){

}

void Log::writeToLog(std::string log, std::ofstream &logFile){
	if (conf.logSetting == "Monitor") {
		LogToMonitor(log);
	}
	else if (conf.logSetting == "File") {
		LogToFile(log, logFile);
	}
	else {
		LogToFile(log, logFile);
		LogToMonitor(log);
	}
}

void Log::LogToFile(std::string log, std::ofstream &logFile) const throw (std::logic_error){
	if (!logFile.is_open()) {
		logFile.open(conf.logPath);
	}
	if (logFile.good()) {
		logFile << log << std::endl;
	}
	else {
		throw std::logic_error("Could not log because of bad file path.");
	}
}

void Log::LogToMonitor(std::string log) const{
	std::cout << log << std::endl;
}
