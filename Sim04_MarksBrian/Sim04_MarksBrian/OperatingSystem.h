/**
*	@file OperatingSystem.h
*	@author Brian Marks
*	@version 1.4
*	@details Class declaration for a simulation of a running operating system
*	@date Wednesday, April 18, 2018
*	@note 1.4 update includes process scheduling
*/

//
// Compiler Guards ////////////////////////
//
#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

//
// Header Files ///////////////////////////
//
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Config.h"
#include "Log.h"
#include "ProcessControlBlock.h"
#include "ResourceManager.h"

extern Config conf;
extern Log logger;

typedef ProcessControlBlock::Operation Operation;
typedef ProcessControlBlock::State State;

//
// Class Delcarations /////////////////////
//

// Meta-Data item container structure
struct MetaDataItem {
	char code;
	std::string descriptor;
	int timeVal;
};

class OperatingSystem {
public:
	// Constructors
	OperatingSystem();				// Default 

	// Initialization functions
	void processMetaData() throw (std::logic_error);						// Processing for full queue of meta-data commands
	bool handleApplicationCode(MetaDataItem md) throw (std::logic_error);	
	void addOperation(ProcessControlBlock &process, MetaDataItem newOp);	// Add operation to a process' operationsQueue

	// Scheduler functions
	void scheduleProcesses() throw (std::logic_error);
	unsigned int priorityIndex();
	void setScheduled(ProcessControlBlock &process) const;
	
	// Simulator functions
	void runSimulation() throw (std::logic_error);

	void setReady(ProcessControlBlock process) throw (std::logic_error);	// Process State setters
	void setRunning(ProcessControlBlock process) throw (std::logic_error);
	void setWaiting(ProcessControlBlock process) throw (std::logic_error);
	void setExit(ProcessControlBlock process) throw (std::logic_error);

private:
	// Container of all processes (set of meta-data codes from A{begin}0; to A{finish}0;)
	std::vector<ProcessControlBlock> processQueue;

	// Schedule for process execution
	std::vector<unsigned int> processSchedule;

	// Resource manager
	ResourceManager resourceManager;
	
	// Control statuses
	bool systemStarted;
	bool systemExited;
	bool readingApplication;
	bool metaDataProcessed;
	int processCount;

	// Meta-Data handling class 
	class MetaData {
	public:
		// Constructors
		MetaData();			// Default

		// Gets
		bool getInitializedStatus() const;

		// Sets
		void MetaDataInit(std::string metaDataFilename) throw(std::logic_error);

		// Additional Functions
		char ReadCode(std::ifstream& fin) throw(std::logic_error);
		std::string ReadDescriptor(std::ifstream& fin, char delimiter) throw(std::logic_error);
		int CalculateRunTime(char metaCode, std::string metaDescriptor, int metaTime) const;

		// Debugging functions
		//void ShowMetaData(std::ofstream & fout, std::string loggingSetting) const;

		// Individual Meta-Data Item
		MetaDataItem metaDataItem;

		// Container of all Meta-Data items
		std::vector<MetaDataItem> metaDataItems;

	private:

		bool metaDataInitialized;

		// Error handling data items
		char codes[6] = { 'S', 'A', 'P', 'I', 'O', 'M' };
		std::string descriptors[12] = { "begin", "finish", "hard drive",
			"keyboard", "scanner", "monitor",
			"run", "allocate", "projector",
			"block", "Start Program Meta-Data Code:",
			"End Program Meta-Data Code" };

	};	// End of Meta-Data class

};	// End of Operating System class


#endif // !OPERATINGSYSTEM_H