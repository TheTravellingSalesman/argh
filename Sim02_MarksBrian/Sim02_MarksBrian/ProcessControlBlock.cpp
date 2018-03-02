/**
*	@file ProcessControlBlock.cpp
*	@author Brian Marks
*	@version 1.0
*	@brief This is the implementation file for the ProcessControlBlock class. The result will be a running process for the simulated operating system.
*	@details Handles generation of threads used for processes, input, and output. Stores information about process and handles process functionality.
*	@date Monday, Feb. 26, 2018
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

typedef ProcessControlBlock::State State;

void* uSleepThread(void* threadarg) {
	long* sleepTimeMilliSec = (long*)threadarg;
	long sleepTimeMicroSec =  (*sleepTimeMilliSec) * 1000;

	Timer threadTimer;

	threadTimer.start();

	while (threadTimer.getElapsedMicroSeconds() < sleepTimeMicroSec);

	return NULL;
}

void ProcessControlBlock::changeState(State newState){
	processState = newState;
}

bool ProcessControlBlock::run() throw (std::logic_error){
	Operation* anOp;
	pthread_t ioThread;

	for (unsigned int i = 0; i < OperationsQueue->size(); i++) {
		anOp = &(*OperationsQueue)[i];

		// Create a thread if the operation is for I/O
		if (anOp->code == 'I' || anOp->code == 'O') {
			// Set process to WAITING
			processState = WAITING;
			
			// Log: Process (pid): start (anOp->descriptor) (anOp->type)
			logger.writeWithTimestamp("Process " + std::to_string(processID) + ": start " + anOp->descriptor + " " + anOp->type);

			long runTime = getRunTimeInMilliSeconds(*anOp);				// Get run time for operation in milliseconds
			runTime = runTime * 1000;									// Convert to microseconds
			void* runningTime = (void*)runTime;							// Explicitly cast run time to (void*)
			pthread_create(&ioThread, NULL, uSleepThread, runningTime);
			pthread_join(ioThread, NULL);
			
			// Log: Process (pid): end (anOp->descriptor) (anOp->type)
			logger.writeWithTimestamp("Process " + std::to_string(processID) + ": end " + anOp->descriptor + " " + anOp->type);

			// Set back to RUNNING
			processState = RUNNING;
		}
		else{
			RunOperation(*anOp);
		}
	}

	return true;
}

void ProcessControlBlock::addOperation(Operation newOp){
	OperationsQueue->push_back(newOp);
}

int ProcessControlBlock::getPID() const{
	return processID;
}

long ProcessControlBlock::getRunTimeInMilliSeconds(Operation operation) const{
	return operation.time * conf.GetOperationTime(operation.code, operation.descriptor);
}

State ProcessControlBlock::getState() const{
	return processState;
}

void ProcessControlBlock::RunOperation(Operation operation){
	if (operation.type == "memory") {
		HandleMemoryOperation(operation);
	}
	else {
		// Log: (ts) Process (pid): start (operation)
		logger.writeWithTimestamp("Process " + std::to_string(processID) + ": start " + operation.type);

		// Sleep for required time
		long runTime = getRunTimeInMilliSeconds(operation);		// Get run time for operation in milliseconds
		runTime = runTime * 1000;								// Convert to microseconds
		void* runningTime = (void*)runTime;						// Explicitly cast run time to (void*)
		uSleepThread(runningTime);								// Sleep for as long as necessary

		// Log: (ts) Process (pid): start (operation)
		logger.writeWithTimestamp("Process " + std::to_string(processID) + ": end " + operation.type);

	}
}

void ProcessControlBlock::HandleMemoryOperation(Operation operation){
	if (operation.descriptor == "allocate") {
		// Seed Random Number Generator for memory address generation
		srand(time(NULL));
		long address = (rand() % conf.GetKbytesAvailable());
		// Log: (ts) Process (pid): (operation.type) 0x(address::hex)
		logger.writeWithTimestamp("Process " + std::to_string(processID) + ": " + operation.type + "0x" + std::to_string(address));
	}
	else {

		logger.writeWithTimestamp("Process " + std::to_string(processID) + ": start memory blocking");

		// Sleep for required time
		long runTime = getRunTimeInMilliSeconds(operation);		// Get run time for operation in milliseconds
		runTime = runTime * 1000;								// Convert to microseconds
		void* runningTime = (void*)runTime;						// Explicitly cast run time to (void*)
		uSleepThread(runningTime);								// Sleep for as long as necessary

		logger.writeWithTimestamp("Process " + std::to_string(processID) + ": end memory blocking");
	}
}

