#include "ResourceManager.h"

/**	Constructor
*	\n Creates a new resource manager object and initializes its counts to 0,
*	also initializes its resource quantities.
*	@pre configInfo must be initialized.
*/
ResourceManager::ResourceManager(){
	initializeResources();
	projectorCount = 0;
	hardDriveCount = 0;
	memoryCount = 0;
}

/** Initialize Resources
*	\n Sets the resource quantities for each resource specified in the config file.
*	@pre configInfo must be initialized.
*/
void ResourceManager::initializeResources(){
	// Parse config for resource info
	for (unsigned int i = 0; i < conf.configInfo.size(); i++) {
		// System memory
		if (conf.configInfo[i].first.substr(0, 13) == "system memory") {
			memory = GetKbytesAvailable(i);
		}
		// Block size
		if (conf.configInfo[i].first.substr(0, 17) == "memory block size") {
			blockSize = GetKbytesBlock(i);
		}
		// Projectors
		if (conf.configInfo[i].first.substr(0, 18) == "projector quantity") {
			projectors = conf.configInfo[i].second;
		}
		// Hard drives
		if (conf.configInfo[i].first.substr(0, 19) == "hard drive quantity") {
			hardDrives = conf.configInfo[i].second;
		}
	}

	// Initialize resource locks
	lock.InitializeLocks(projectors, hardDrives, memory, blockSize);
}

/** Get Available kBytes for memory
*	\n A getter function for the kBytes available in system memory
*	@return the amount of space available in kbytes
*	@pre configInfo must be initialized
*	@throw couldn't determine space available
*/
unsigned long ResourceManager::GetKbytesAvailable(unsigned int element) const throw(std::logic_error) {
	if (conf.configInfo[element].first[15] == 'k') {
		return conf.configInfo[element].second;
	}
	else if (conf.configInfo[element].first[15] == 'M') {
		return (conf.configInfo[element].second * 1000);
	}
	else if (conf.configInfo[element].first[15] == 'G') {
		return (conf.configInfo[element].second * 1000000);
	}
	else {
		throw std::logic_error("Could not determine system memory size in kbytes, Mbytes, or Gbytes; Check config file.");
	}
}

/** Get Available kBytes for block size
*	\n A getter function for the size of each block in kbytes
*	@return the block size in kbytes
*	@pre configInfo must be initialized
*	@throw couldn't determine block size
*/
unsigned long ResourceManager::GetKbytesBlock(unsigned int element) const throw(std::logic_error) {
	if (conf.configInfo[element].first[19] == 'k') {
		return conf.configInfo[element].second;
	}
	else if (conf.configInfo[element].first[19] == 'M') {
		return (conf.configInfo[element].second * 1000);
	}
	else if (conf.configInfo[element].first[19] == 'G') {
		return (conf.configInfo[element].second * 1000000);
	}
	else {
		throw std::logic_error("Could not determine memory block size in kbytes, Mbytes, or Gbytes; Check config file.");
	}
}

/**	Check and Set Projector
*	\n Checks the current count for projectors in use, then sets the next available projector as in use, then returns the projector number.
*	When the available projectors are exhausted, the next projector used is projector 0.
*	@return the number of the projector being allocated
*/
unsigned int ResourceManager::CheckSetProjector(){
	return (projectorCount++ % projectors);
}

/**	Check and Set Hard Drive
*	\n Checks the current count for hard drives in use, then sets the next available hard drive as in use, then returns the hard drive number.
*	When the available hard drives are exhausted, the next hard drive used is hard drive 0.
*	@return the number of the hard drive being allocated
*/
unsigned int ResourceManager::CheckSetHardDrive(){
	return (hardDriveCount++ % hardDrives);
}

/**	Check and Set Memory
*	\n Checks the current count for memory blocks in use, then sets the next available memory block as in use, then returns the beginning of the memory block address.
*	If all memory blocks are locked and in use, this will wait for the next available block to be available, which it will then lock and allocate.
*	@throw Runtime error is thrown if it takes more than 10 seconds to find an available memory slot. This error is likely a result of 
*			a single process requiring more memory than the system has. When processes are run concurrently, an exceptionally long process
*			could also be the cause of this error.
*	@return the number of the memory block being allocated
*/
unsigned long ResourceManager::CheckSetMemory() throw (std::runtime_error){
	Timer searching;
	searching.start();
	// Find and lock an available memory address
	for ( ; lock.TestAndSetMemory(memoryCount); memoryCount++) {
		// If no available slot has been found after 10 seconds, throw a runtime error
		if (searching.getElapsedSeconds() > 10) {
			throw std::runtime_error("Memory allocation time exceeded 10s; One process likely requires memory in excess of what is available in the system.");
		}
	}
		
	return ((memoryCount++ * blockSize) % memory);
}

/**	Get Block Size
*	\n Getter function for the size of the memory blocks
*	@return the size of the memory blocks
*/
unsigned long ResourceManager::GetBlockSize(){
	return blockSize;
}
