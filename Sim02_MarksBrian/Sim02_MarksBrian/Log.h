/**
*	@file Config.h
*	@author Brian Marks
*	@version 1.2
*	@details Class declaration for the storing and handling of Configuration file data
*	@note 1.2 update: Adjusted ReadKey to be compatible with new configuration file format/descriptor options.
*	@date Monday, Feb. 26, 2018
*/

//
// Compiler Guards ////////////////////////
//
#ifndef LOG_H
#define LOG_H

//
// Header Files ///////////////////////////
//
#include <iostream>
#include <fstream>
#include "Config.h"

extern Config conf;

//
// Class Function Declarations ////////////
//
class Log {
public:
	// Constructors
	Log();

	// Functions
	void writeToLog(std::string log);
	void LogToFile(std::string log, std::ofstream &logFile) const throw (std::logic_error);
	void LogToMonitor(std::string log) const;
	void CloseLogPath(std::string logPath);

private:
	
};


#endif // !LOG_H