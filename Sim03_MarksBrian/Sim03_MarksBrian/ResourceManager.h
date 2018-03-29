/**
*	@file Resource Manager
*	@author Brian Marks
*	@version 1.0
*	@details Class declaration for a resource manager which manages the available resources
*	@date Wednesday, March 28, 2018
*/

//
// Compiler Guards ////////////////////////
//
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

//
// Header Files ///////////////////////////
//
#include <stdexcept>
#include "Config.h"

extern Config conf;

//
// Class Declaration ///////////////////////////
//
class ResourceManager{
public:
	// Constructor
	ResourceManager();

	// Initialization functions
	void initializeResources();
	unsigned long GetKbytesAvailable(unsigned int element) const throw(std::logic_error);
	unsigned long GetKbytesBlock(unsigned int element) const throw(std::logic_error);

	// Accessors
	unsigned int CheckSetProjector();
	unsigned int CheckSetHardDrive();
	unsigned long CheckSetMemory();

private:
	// Resource quantities
	unsigned int projectors;
	unsigned int hardDrives;
	unsigned long memory;
	unsigned long blockSize;

	// Resource counts
	unsigned int projectorCount;
	unsigned int hardDriveCount;
	unsigned long memoryCount;
};

#endif	// !RESOURCEMANAGER_H

