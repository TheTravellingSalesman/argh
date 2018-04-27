#include "Lock.h"

/**	Constructor
*	\n Creates a new mutex lock object
*/
Lock::Lock(){
	mutexLockState = false;
	
	
}

/**	Initialize Locks
*	\n Initializes all of the semaphore locks used for the manageable resources. Locks are initially unlocked.
*	@param All parameters are the quantities of the manageable resources for which the locks are being made.
*/
void Lock::InitializeLocks(unsigned int projectors, unsigned int hardDrives, unsigned long memory, unsigned long blockSize){
	// Initialize projector semaphore for each projector
	for (unsigned int i = 0; i < projectors; i++) {
		projectorLocks.push_back(false);
	}

	// Initialize hard drive semaphore for each hard drive
	for (unsigned int i = 0; i < hardDrives; i++) {
		hardDriveLocks.push_back(false);
	}

	// Initialize memory block semaphores for each memory block
	unsigned long numBlocks = memory / blockSize;
	for (unsigned long i = 0; i < numBlocks; i++) {
		memoryBlockLocks.push_back(false);
	}
}

/**	Unlock
*	\n Lock the mutex
*/
void Lock::UnlockMutex(){
	mutexLockState = false;
}

/**	Unlock Projector
*	\n Unlocks a specified projector semaphore
*	@param index specifies the projector which is being deallocated
*/
void Lock::UnlockProjector(const unsigned int index){
	projectorLocks[index] = false;
}

/**	Unlock Hard Drive
*	\n Unlocks a specified hard drive semaphore
*	@param index specifies the hard drive which is being deallocated
*/
void Lock::UnlockHardDrive(const unsigned int index){
	hardDriveLocks[index] = false;
}

/**	Unlock Memory
*	\n Unlocks a specified memory block semaphore
*	@param index specifies the memory block which is being deallocated
*/
void Lock::UnlockMemory(const unsigned int index){
	memoryBlockLocks[index] = false;
}

/**	Test and Set Mutex
*	\n Checks the state of the lock, then locks it
*	@return true if the mutex was locked, false otherwise
*/
bool Lock::TestAndSetMutex(){
	bool initialState = mutexLockState;
	mutexLockState = true;
	
	return initialState;
}

/**	Test and Set Projector
*	\n Checks the state of the specified projector semaphore, then locks it
*	@param index specifies the projector being checked
*	@return true if the projector was locked, false otherwise
*/
bool Lock::TestAndSetProjector(const unsigned int index){
	bool initialState = projectorLocks[index];
	projectorLocks[index] = true;

	return initialState;
}

/**	Test and Set Hard Drive
*	\n Checks the state of the specified hard drive semaphore, then locks it
*	@param index specifies the hard drive being checked
*	@return true if the hard drive was locked, false otherwise
*/
bool Lock::TestAndSetHardDrive(const unsigned int index){
	bool initialState = hardDriveLocks[index];
	hardDriveLocks[index] = true;

	return initialState;
}

/**	Test and Set Memory
*	\n Checks the state of the specified memory block semaphore, then locks it
*	@param index specifies the memory block being checked
*	@return true if the memory block was locked, false otherwise
*/
bool Lock::TestAndSetMemory(const unsigned int index){
	bool initialState = memoryBlockLocks[index];
	memoryBlockLocks[index] = true;

	return initialState;
}
