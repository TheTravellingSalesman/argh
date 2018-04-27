/**
*	@file Lock
*	@author Brian Marks
*	@version 1.4
*	@details Class declaration for a set of mutex and semaphore locks which work with pthreads
*	@date Wednesday, April 18, 2018
*	@note 1.4 update added semaphore functionality for each manageable resource
*/

//
// Compiler Guards ////////////////////////
//
#ifndef LOCKS_H
#define LOCKS_H

//
// Header Files /////////////////////////////
//
#include "Config.h"
#include <vector>

extern Config conf;

//
// Class Declaration ///////////////////////////
//
class Lock{
public:
	// Constructor
	Lock();

	// Initializer
	void InitializeLocks(unsigned int projectors, unsigned int hardDrives, unsigned long memory, unsigned long blockSize);

	// Status functions
	void UnlockMutex();
	void UnlockProjector(const unsigned int index);
	void UnlockHardDrive(const unsigned int index);
	void UnlockMemory(const unsigned int index);
	bool TestAndSetMutex();
	bool TestAndSetProjector(const unsigned int index);
	bool TestAndSetHardDrive(const unsigned int index);
	bool TestAndSetMemory(const unsigned int index);

private:
	// Mutex Lock
	bool mutexLockState;

	// Semaphore Locks
	std::vector<bool> projectorLocks;
	std::vector<bool> hardDriveLocks;
	std::vector<bool> memoryBlockLocks;

};

#endif // ! LOCKS
