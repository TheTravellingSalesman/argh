/**
*	@file Timer.h
*	@author Brian Marks
*	@version 1.0
*	@details Class definition for a basic timer used for the simulated operating system.
*	@date Monday, Feb. 26, 2018
*/

//
// Compiler Guards ////////////////////////
//
#ifndef TIMER_H
#define TIMER_H

//
// Header Files /////////////////////////////
//
#include <sys/time.h>
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <iostream>

//
// Class Function Declarations ////////////
//
class Timer {
public:
	// Constructors
	Timer();									// Default

	// Functions
	void start() throw (std::runtime_error);	
	void stop() throw (std::logic_error);

	// Accessors
	long double getElapsedSeconds();
	long double getElapsedMilliSeconds();
	long double getElapsedMicroSeconds();

private:
	// Data items
	struct timeval beginTime;
	struct timeval duration;
	long double startTime;
	bool timerWasStarted;
};
#endif	// !TIMER_H