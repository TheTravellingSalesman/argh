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
#include <sstream>
#include <iomanip>
#include "Config.h"
#include "Timer.h"

extern Config conf;

//
// Class Function Declarations ////////////
//
class Log {
public:
	// Constructors
	Log();
	
	// Functions
	void initializeLogSettings();
	void writeToLog(std::string log);
	void writeWithTimestamp(std::string log);

	void streamToFile() throw (std::logic_error);

private:
	Timer logTimer;

	// Control data
	bool initialized, logToMonitor, logToFile;
	std::ostringstream outstream;
};


#endif // !LOG_H