/**
*	@file Sim03.cpp
*	@mainpage Sim03
*	@author Brian Marks
*	@version 1.3
*	@details Second phase of a simulation of an operating system. This phase adds resource management to the previous functionality.
*	@date Wednesday, March 28, 2018
*/

//
// Header Files ///////////////////////
//

#include <iostream>
#include <unistd.h>
#include "Config.h"
#include "Log.h"
#include "OperatingSystem.h"
#include "MutexLock.h"

// Global declaration of shared classes
Config conf;
Log logger;
MutexLock mutex;

//
// Main Function Implementation
//
int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cout << "Error: Can only run exactly one file name passed as a command line parameter." << std::endl;		// Throw error if there is not exactly one argument in command line
		exit(0);
	}

	conf.ConfigInit(argv[1]);		// Initialize config information

	OperatingSystem os;				// Initialize the operating system

	os.runSimulation();				// Run the simulation

	logger.streamToFile();			// Log the results

	return 0;
}