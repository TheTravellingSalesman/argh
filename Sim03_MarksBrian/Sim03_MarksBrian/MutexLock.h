/**
*	@file MutexLock
*	@author Brian Marks
*	@version 1.0
*	@details Class declaration for a mutex lock which works with pthreads
*	@date Wednesday, March 28, 2018
*/

//
// Compiler Guards ////////////////////////
//
#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

//
// Class Declaration ///////////////////////////
//
class MutexLock{
public:
	// Constructor
	MutexLock();

	// Status functions
	void Unlock();
	bool TestAndSet();

private:
	// Lock
	bool lockState;

};

#endif // ! MUTEXLOCK
