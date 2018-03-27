#include "Log.h"
/** Default Log constructor
*	\n Creates a new log
*/
Log::Log(){
	logToMonitor = false;
	logToFile = false;
}

/**	Initialize Log Settings
*	\n Sets all the Log control information as is appropriate.
*	@return true if initialized, false if not
*/
void Log::initializeLogSettings(){
	// set log settings based on config
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

	// start log timer
	logTimer.start();

	// set initialized
	initialized = true;
}

/**	Write to Log
*	\n Logs a simple message.
*	@param log is the string message to be output
*/
void Log::writeToLog(std::string log){
	if (logToMonitor) {
		std::cout << log << std::endl;
	}
	if (logToFile) {
		outstream << log << std::endl;
	}
}

/**	Write to Log with Timestamp
*	\n Logs a simple message with a timestamp, precise down to ms.
*	@param log is the string message to be output
*/
void Log::writeWithTimestamp(std::string log){
	if (logToMonitor) {
		std::cout << std::fixed << std::setprecision(6) << logTimer.getElapsedSeconds() << " - " << log << std::endl;
	}
	if (logToFile) {
		outstream << std::fixed << std::setprecision(6) << logTimer.getElapsedSeconds() << " - " << log << std::endl;
	}
}

/**	Write to Log with Address
*	\n Logs a simple message, followed by a number representing a memory address in hexadecimal.
*	@param log is the string message to be output
*	@param address is the long which will be converted to hex. This represents a memory address.
*/
void Log::writeWithAddress(std::string log, long address){
	if (logToMonitor) {
		std::cout << std::fixed << std::setprecision(6) << logTimer.getElapsedSeconds() << " - " << log << std::setfill('0') << std::setw(8) << std::hex << address << std::endl;
	}
	if (logToFile) {
		outstream << std::fixed << std::setprecision(6) << logTimer.getElapsedSeconds() << " - " << log << std::setfill('0') << std::setw(8) << std::hex << address << std::endl;
	}
}

/**	Stream to File
*	\n Drains the output buffer to a log file.
*	@pre log file path must be specified by config in main before it can be logged 
*	@throw log file is bad.
*/
void Log::streamToFile() throw(std::logic_error) {
		std::ofstream fout;

		fout.open(conf.logPath.c_str(), std::ofstream::out);
		if(fout.good()){
			fout << outstream.str();

			fout.close();
		}
		else {
			throw std::logic_error("streamToFile(): Bad log file");
		}
	
}