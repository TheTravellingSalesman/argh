/**
*	@file Sim01.cpp
*	@mainpage Sim01
*	@author Brian Marks
*	@version 1.1
*	@details First phase of a simulation of an operating system. This phase handles the reading of configuration data from a file,
*	which it stores, and runs operations upon. Within the configuration data lies a file directory for meta-data information
*	which is also stored, and plays a role in data operations. The configuration data contains the timing per cycle for various
*	meta-data processes, then the meta-data contains the processes which are run, and for how many cycles it will run.
*	@date Monday, Feb. 26, 2018
*/

//
// Header Files ///////////////////////
//

#include "Config.h"
#include "MetaData.h"

//
// Main Function Implementation
//
int main(int argc, char* argv[]) {
	Config conf;
	MetaData md;
	std::ifstream metaDataFile;
	std::ofstream logFile;

	if (argc != 2) {
		std::cout << "Error: Can only run exactly one file name passed as a command line parameter." << std::endl;		// Throw error if there is not exactly one argument in command line
		exit(0);
	}

	conf.ConfigInit(argv[1]);		// Initialize config information

	md.MetaDataInit(metaDataFile, conf.metaDataFilename);	// Initialize meta-data information

	/*	I/O for Sim01
	
	conf.ShowConfig(logFile);	// Show config info

	md.ShowMetaData(logFile, conf.configInfo, conf.GetLogSetting());	// Show meta-data info
	
	*/
	return 0;
}