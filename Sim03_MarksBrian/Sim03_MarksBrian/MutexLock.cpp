#include "MutexLock.h"

/**	Constructor
*	\n Creates a new mutex lock object
*/
MutexLock::MutexLock(){
	lockState = false;
}

/**	Unlock
*	\n Locks the mutex
*/
void MutexLock::Unlock(){
	lockState = false;
}

/**	Test and Set
*	\n Checks the state of the lock, then locks it
*	@return true if the mutex was locked, false otherwise
*/
bool MutexLock::TestAndSet(){
	bool initialState = lockState;
	lockState = true;
	
	return initialState;
}
