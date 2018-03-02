/**
*	@file Timer.cpp
*	@author Brian Marks
*	@version 1.0
*	@brief This is the implementation file for the Timer class. The result will be the measured time that it takes for a process to be completed.
*	@details The Timer class acts as a "stopwatch", in that it starts timing at the beginning of an event, and stops timing upon completion of the event, 
*	then returns the difference in time between the end and beginning.
*	@date Monday, Feb. 26, 2018
*/

/**
*	@mainpage
*	This is a short and simple class implementation which handles the following functionality:
*	- Starting the timer
*	- Stopping the timer
*	- Measuring and returning the elapsed time between start and stop.
*	Each function also contains error and exception handling, which stops the program when triggered.
*/

/*
*	Author self-assessment statement:
*	This program is complete and working according to specifications, according to the best of my knowledge.
*/

//
// Header Files ////////////////////////////////////////
//

#include "Timer.h"
#include <sys/time.h>

//
// Timer Member Function Implementations ////////////////
//

/** Default constructor
* Initializes a Timer objects timer values, which prepares
* the object to measure time.
*/
Timer::Timer() {
	timerWasStarted = false;
}

/** Engage the timer.
* Stores the beginning of the time interval.
* @pre The timer has not been engaged.
* @throw The timer was already running.
*/
void Timer::start() throw (std::runtime_error) {
	if (timerWasStarted == true) {
		throw std::runtime_error("start() timer is already running");
	}
	else {
		gettimeofday(&beginTime, NULL);
		timerWasStarted = true;
	}
}

/** Stop the timer.
* Stores the end of the time interval.
* @pre The beginning of the interval has not been stored yet.
* @throw The timer was not running.
*/
void Timer::stop() throw (std::logic_error) {
	if (timerWasStarted == false) {
		throw std::logic_error("stop() timer is not running");
	}
	else {
		struct timeval stopTime;
		gettimeofday(&stopTime, NULL);
		duration.tv_sec = (stopTime.tv_sec - beginTime.tv_sec);
		duration.tv_usec = (stopTime.tv_usec - beginTime.tv_usec);
		timerWasStarted = false;
	}
}

/** Measure and return the difference in time between the start
* and stop of the timer (in seconds).
* @pre The beginning and the end of the interval have been stored.
* @throw The timer has either not been started, or is still running.
* @return The time elapsed between the beginning and end of the timer, in seconds.
*/
long double Timer::getElapsedSeconds() const{
		long double elapsedTime = (duration.tv_sec * 1000000 + duration.tv_usec - startTime);
		elapsedTime = elapsedTime / 1000000;
		return elapsedTime;
}

long double Timer::getElapsedMilliSeconds() const{
		long double elapsedTime = (duration.tv_sec * 1000000 + duration.tv_usec - startTime);
		elapsedTime = elapsedTime / 1000;
		return elapsedTime;
}

long double Timer::getElapsedMicroSeconds() const{
		long double elapsedTime = (duration.tv_sec * 1000000 + duration.tv_usec - startTime);
		return elapsedTime;
}

