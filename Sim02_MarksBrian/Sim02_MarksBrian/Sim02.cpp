/**
*	@file Sim02.cpp
*	@mainpage Sim02
*	@author Brian Marks
*	@version 1.2
*	@details First phase of a simulation of an operating system. This phase handles the simulation of running a single process within the system.
*	@date Monday, Feb. 26, 2018
*/

//
// Header Files ///////////////////////
//

#include <iostream>
#include <unistd.h>
#include "Config.h"
#include "Log.h"
#include "OperatingSystem.h"

// Global declaration of shared classes
Config conf;
Log logger;

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