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
	Timer();
	void start() throw (std::runtime_error);
	void stop() throw (std::logic_error);
	long double getElapsedSeconds() const;
	long double getElapsedMilliSeconds() const;
	long double getElapsedMicroSeconds() const;
private:
	struct timeval beginTime;
	struct timeval duration;
	long double startTime;
	bool timerWasStarted;
};
#endif	// !TIMER_H