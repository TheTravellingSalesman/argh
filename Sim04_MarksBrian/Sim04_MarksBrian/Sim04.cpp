/**
*	@file Sim04.cpp
*	@mainpage Sim04
*	@author Brian Marks
*	@version 1.4
*	@details Fourth phase of a simulated operating system. This phase adds scheduling and multiple process exectuion to the previous implementation.
*	@date Wednesday April 19, 2018
*/

//
// Header Files ///////////////////////
//

#include <iostream>
#include <unistd.h>
#include "Config.h"
#include "Log.h"
#include "OperatingSystem.h"
#include "Lock.h"

// Global declaration of shared classes
Config conf;
Log logger;
Lock lock;

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