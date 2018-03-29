/**
*	@file ProcessControlBlock.cpp
*	@author Brian Marks
*	@version 1.0
*	@brief This is the implementation file for the ProcessControlBlock class. The result will be an executable process for the simulated operating system.
*	@details Handles generation of threads used for processes, input, and output. Stores information about process and handles process functionality.
*	@date Wednesday, March 28, 2018
*/

/**
*	@mainpage
*	This is a class implementation which handles the following functionality:
*	-	Generate a new process.
*	-	Create threads for process functionalities.
*	-	Output process information.
*/

/*
*	Author self-assessment statement:
*	This program is complete and working according to specifications, according to the best of my knowledge.
*/

//
// Header Files ////////////////////////////////////////
//
#include <stdlib.h>
#include <time.h>
#include "ProcessControlBlock.h"
#include "Config.h"

// Forward declaration of state of process type
typedef ProcessControlBlock::State State;

/**	Microsecond Sleep Thread
*	\n A process that can be called in the creation of a thread which sleeps
*	@param threadarg is the time for which the thread will sleep
*/
void* uSleepThread(void* threadarg) {
	// Take in the parameter, which is the time this process will sleep
	long sleepTimeMicroSec = (long)threadarg;

	// start timer
	Timer threadTimer;

	threadTimer.start();

	// SLEEEP
	while (threadTimer.getElapsedMicroSeconds() < sleepTimeMicroSec);

	return NULL;
}

/**	Change State
*	\n Changes the state of the process to a different state.
*/
void ProcessControlBlock::changeState(State newState){
	processState = newState;
}

/** Run Process
*	\n Executes the process from beginning to end
*	@pre OperationsQueue must be filled with operations for the process to complete.
*/
bool ProcessControlBlock::run(ResourceManager &rm){
	Operation* anOp;

	// Loop through all processes that need to be completed, in order
	for (unsigned int i = 0; i < OperationsQueue.size(); i++) {
		anOp = &OperationsQueue[i];

		// Create a thread if the operation is for I/O
		if (anOp->code == 'I' || anOp->code == 'O') {
			pthread_t ioThread;
			// Set process to WAITING
			processState = WAITING;

			// Wait for the thread to be free
			while (mutex.TestAndSet() == 1);

				// CRITICAL SECTION

				// Log differs based on device
				if (anOp->descriptor == "projector"){
					// Log: Process (pid): start (anOp->descriptor) (anOp->type) on PROJ (rm.CheckSetProjector)
					logger.writeWithTimestamp("Process " + std::to_string(processID + 1) + ": start " + anOp->descriptor 
												+ anOp->type + " on PROJ " + std::to_string(rm.CheckSetProjector()));
				}
				else if (anOp->descriptor == "hard drive") {
					// Log: Process (pid): start (anOp->descriptor) (anOp->type) on HDD (rm.CheckSetHardDrive)
					logger.writeWithTimestamp("Process " + std::to_string(processID + 1) + ": start " + anOp->descriptor
						+ anOp->type + " on HDD " + std::to_string(rm.CheckSetHardDrive()));
				}
				else {
					// Log: Process (pid): start (anOp->descriptor) (anOp->type)
					logger.writeWithTimestamp("Process " + std::to_string(processID + 1) + ": start " + anOp->descriptor + anOp->type);
				}

				long runTime = getRunTimeInMilliSeconds(*anOp);				// Get run time for operation in milliseconds
				runTime = runTime * 1000;									// Convert to microseconds
				void* runningTime = (void*)runTime;							// Explicitly cast run time to (void*)
				pthread_create(&ioThread, NULL, uSleepThread, runningTime);

			// Unlock the thread
			mutex.Unlock();

			pthread_join(ioThread, NULL);
			
			// Log: Process (pid): end (anOp->descriptor) (anOp->type)
			logger.writeWithTimestamp("Process " + std::to_string(processID+1) + ": end " + anOp->descriptor + anOp->type);

			// Set back to RUNNING
			processState = RUNNING;
		}
		// Otherwise just run the operation
		else{
			RunOperation(*anOp, rm);
		}
	}
	// Process executed successfully!
	return true;
}

/** Add Operation
*	\n Adds an operation to the Operations queue vector
*	@param newOp is the operation to push onto the queue, passed by ref.
*/
void ProcessControlBlock::addOperation(Operation& newOp){	
	OperationsQueue.push_back(newOp);
}

/** Get PID
*	\n Process ID accessor.
*	@return process ID.
*/
int ProcessControlBlock::getPID() const{
	return processID;
}

/** Get Run Time in Milliseconds
*	\n Calculates and returns the run time for a given operation based on the configuration data for the operating system.
*	@pre Configuration data must be initialized.
*	@param operation who's time is being calculated.
*	@return the run time in milliseconds for the given operation.
*/
long ProcessControlBlock::getRunTimeInMilliSeconds(Operation operation) const{
	return operation.time * conf.GetOperationTime(operation.code, operation.descriptor);
}

/**	Get State
*	\n Process state accessor
*	@return state of the process {START, RUNNING, READY, WAITING, EXIT}
*/
State ProcessControlBlock::getState() const{
	return processState;
}

/** State As String		(***DEBUGGING***)
*	\n Converts the process state (type enum) to a string. For debugging purposes.
*	@return the state of the process as a string
*/
std::string ProcessControlBlock::stateAsString(State state) const{
	if (state == START) {
		return "start";
	}
	if (state == READY) {
		return "ready";
	}
	if (state == RUNNING) {
		return "running";
	}
	if (state == WAITING) {
		return "waiting";
	}
	if (state == EXIT) {
		return "exit";
	}
	else {
		return "NULL";
	}
}

/** Print Operations Queue		(***DEBUGGING***)
*	\n Prints out all of the operations in the operations queue.
*/
void ProcessControlBlock::printOperationsQueue() const{
	for (unsigned int i = 0; i < OperationsQueue.size(); i++) {
		std::cout << OperationsQueue[i].code << " " << OperationsQueue[i].type << " " << OperationsQueue[i].descriptor << " " << OperationsQueue[i].time << std::endl;
	}
}

/** Run Operation
*	\n Run one operation (simulation).
*	@param operation to be run
*/
void ProcessControlBlock::RunOperation(Operation operation, ResourceManager &rm){

	if (operation.type == "memory allocated at ") {
		HandleMemoryOperation(operation, rm);
	}
	else {
		// Log: (ts) Process (pid): start (operation)
		logger.writeWithTimestamp("Process " + std::to_string(processID+1) + ": start " + operation.type);

		// Sleep for required time
		long runTime = getRunTimeInMilliSeconds(operation);		// Get run time for operation in milliseconds
		runTime = runTime * 1000;								// Convert to microseconds
		void* runningTime = (void*)runTime;						// Explicitly cast run time to (void*)
		uSleepThread(runningTime);								// Sleep for as long as necessary

		// Log: (ts) Process (pid): start (operation)
		logger.writeWithTimestamp("Process " + std::to_string(processID+1) + ": end " + operation.type);

	}
}

/**	Handle Memory Operation
*	\n Handles operations on memory. Sets a random memory addresss when allocating memory.
*	@param operation to be run
*/
void ProcessControlBlock::HandleMemoryOperation(Operation operation, ResourceManager &rm){
	if (operation.descriptor == "allocate") {
		logger.writeWithTimestamp("Process " + std::to_string(processID + 1) + ": allocating memory" );

		long address = rm.CheckSetMemory();
		// Log: (ts) Process (pid): (operation.type) 0x(address::hex)
		logger.writeWithAddress("Process " + std::to_string(processID+1) + ": " + operation.type + "0x", address);
	}
	else {

		logger.writeWithTimestamp("Process " + std::to_string(processID+1) + ": start memory blocking");

		// Sleep for required time
		long runTime = getRunTimeInMilliSeconds(operation);		// Get run time for operation in milliseconds
		runTime = runTime * 1000;								// Convert to microseconds
		void* runningTime = (void*)runTime;						// Explicitly cast run time to (void*)
		uSleepThread(runningTime);								// Sleep for as long as necessary

		logger.writeWithTimestamp("Process " + std::to_string(processID+1) + ": end memory blocking");
	}
}

