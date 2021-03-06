/**
*	@file Config.h
*	@author Brian Marks
*	@version 1.3
*	@details Class declaration for the storing and handling of Configuration file data
*	@note 1.3 update: Adjusted ReadKey to be compatible with new configuration file format/descriptor options.
*	@date Wednesday, March 28, 2018
*/

//
// Compiler Guards ////////////////////////
//
#ifndef CONFIG_H
#define CONFIG_H

//
// Header Files ///////////////////////////
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

//
// Class Function Declarations ////////////
//
class Config {
public:
	// Constructors
	Config();

	// Gets
	void OpenLogPath(std::ofstream& logFile) throw(std::logic_error);
	std::string GetLogSetting() const;
	int GetOperationTime(char metaCode, std::string metaDescriptor) const throw(std::logic_error);

	// Sets
	void ConfigInit(char* fileIn) throw (std::logic_error);

	// Additional functions
	void SetLogSetting(std::string type) throw(std::logic_error);
	std::string ReadKey(std::ifstream& fin, char delimiter) throw(std::logic_error);
	//void ShowConfig(std::ofstream& fout);

	// Public Data
	std::vector< std::pair<std::string, int> > configInfo;	// vector of pairs<process name, timing> stores all meta-data names and timings
	std::string metaDataFilename;							// Meta Data file path
	std::string logPath;									// Log file path
	std::string logSetting;									// Log to monitor, file, or both
	std::string version;									// Config file version description

private:
	// Error Handling Data Items
	std::string configReads[21] = { "Start Simulator Configuration File",
		"Version/Phase:",
		"File Path",
		"Monitor display time {msec}",
		"Processor cycle time {msec}",
		"Scanner cycle time {msec}",
		"Hard drive cycle time {msec}",
		"Keyboard cycle time {msec}",
		"Memory cycle time {msec}",
		"Projector cycle time {msec}",
		"System memory {kbytes}",
		"System memory {Mbytes}",
		"System memory {Gbytes}",
		"Memory block size {kbytes}",
		"Memory block size {Mbytes}",
		"Memory block size {Gbytes}",
		"Projector quantity",
		"Hard drive quantity", 
		"Log:",
		"Log File Path",
		"End Simulator Configuration Fil" };		// Array holding all possible valid config file key reads (for spell checking)
};

#endif // !CONFIG_H
