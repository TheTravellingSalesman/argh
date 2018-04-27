/**
*	@file ProcessControlBlock.h
*	@author Brian Marks
*	@version 1.4
*	@details Class definition for the process control block which handles process information and control.
*	@date Wednesday, April 18, 2018
*	@note 1.4 update introduces process scheduling control data
*/

//
// Compiler Guards ////////////////////////
//
#ifndef PROCESSCONTROLBLOCK_H
#define PROCESSCONTROLBLOCK_H

//
// Header Files /////////////////////////////
//
#include <string>
#include <vector>
#include <pthread.h>
#include "Timer.h"
#include "Config.h"
#include "Log.h"
#include "Lock.h"
#include "ResourceManager.h"

extern Config conf;		// Forward declaration of global Config item initialized in main 
extern Log logger;		// Forward declaration of global log initialized in main
extern Lock lock; // Forward declaration of global mutex lock 

//
// Class Function Declarations ////////////
//
class ProcessControlBlock {
public:
	
	// Types available for PCB status
	enum State {
		START,
		READY,
		RUNNING,
		WAITING,
		EXIT
	};

	// Struct for information about individual operations within this process
	struct Operation {
		// Parameterized constructor
		Operation(char opCode, std::string opDescription, int cycleTime)
			: code(opCode), descriptor(opDescription), time(cycleTime) {
			codeToType();
		};

		// Copy constructor
		Operation(const Operation& other) :code(other.code), descriptor(other.descriptor), time(other.time) { codeToType(); };

		void codeToType() {
			switch (code) {
				case 'S':
					type = "system";
					break;
				case 'A':
					type = "application";
					break;
				case 'P':
					type = "processing action";
					break;
				case 'I':
					type = " input";
					break;
				case 'O':
					type = " output";
					break;
				case 'M':
					type = "memory allocated at ";
					break;
				default:
					type = "Operation type error";
			}
		}

		char code;
		std::string type;
		std::string descriptor;
		int time;
	};

	// Constructors
	ProcessControlBlock(int pid) : processID(pid), numIO(0), numOps(0), processState(ProcessControlBlock::START), scheduled(false) {};

	// Member functions
	void changeState(State newState);
	bool run(ResourceManager &rm);
	void addOperation(Operation &newOp);
	void setScheduled();
	
	// Accessors
	int getPID() const;
	long getRunTimeInMilliSeconds(Operation operation) const;
	State getState() const;
	std::string stateAsString(State state) const;
	void printOperationsQueue() const;
	unsigned int getNumIO() const;
	unsigned int getNumOps() const;
	bool getScheduledState() const;

private:

	// Private functions
	void RunOperation(Operation operation, ResourceManager &rm);
	void HandleMemoryOperation(Operation operation, ResourceManager &rm);

	// Private data
	int processID;
	int numIO;
	int numOps;
	State processState;
	std::vector<Operation> OperationsQueue;
	std::vector<long> allocatedMemoryIndices;
	bool scheduled;
};

#endif // !PROCESSCONTROLBLOCK_H