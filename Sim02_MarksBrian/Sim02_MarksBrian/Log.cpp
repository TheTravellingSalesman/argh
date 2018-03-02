#include "Log.h"

Log::Log(){
	logToMonitor = false;
	logToFile = false;
}

void Log::initializeLogSettings(){
	if (conf.logSetting == "Monitor") {
		logToMonitor = true;
	}
	else if (conf.logSetting == "Both") {
		logToMonitor = true;
		logToFile = true;
	}
	else {
		logToFile = true;
	}

	logTimer.start();

	initialized = true;
}

void Log::writeToLog(std::string log){
	if (logToMonitor) {
		std::cout << log << std::endl;
	}
	if (logToFile) {
		outstream << log << std::endl;
	}
}

void Log::writeWithTimestamp(std::string log){
	if (logToMonitor) {
		std::cout << std::fixed << std::setprecision(6) << logTimer.getElapsedSeconds() << " - " << log << std::endl;
	}
	if (logToFile) {
		outstream << std::fixed << std::setprecision(6) << logTimer.getElapsedSeconds() << " - " << log << std::endl;
	}
}

void Log::streamToFile() throw(std::logic_error) {
	if (logToFile) {
		std::ofstream fout;

		fout.open(conf.logPath, std::ofstream::out);
		if(fout.good()){
			fout << outstream.rdbuf();

			fout.close();
		}
		else {
			throw std::logic_error("streamToFile(): Bad log file");
		}
	}
}