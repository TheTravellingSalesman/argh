/**
*	@file OperatingSystem.cpp
*	@author Brian Marks
*	@version 1.2
*	@details Class implementation for a simulation of a running operating system
*	@date Monday, Feb. 26, 2018
*/

//
// Header Files ///////////////////////////
//
#include "OperatingSystem.h"

//
// OperatingSystem Class Function Implementations /////////
//

/**	Default Constructor
*	\n Sets up the operating system with it's initial necessary information. Initializes processing of Meta-Data.
*/
OperatingSystem::OperatingSystem() {
	systemStarted = false;
	systemExited = false;
	readingApplication = false;
	metaDataProcessed = false;
	processCount = 0;
	processMetaData();
}

/**	Process Meta Data
*	\n Processes the Meta-Data in the MetaDataItems vector, and assigns operations to processes.
*	@pre MetaDataItems vector must be initialized.
*	@throw some kind of error in meta-data sequence. Details in the throws.
*/
void OperatingSystem::processMetaData() throw (std::logic_error) {
	if (!metaDataProcessed) {
		MetaData md;

		if (md.getInitializedStatus()) {
			// Parse through all existing meta-data items and handle them appropriately
			for (unsigned int i = 0; i < md.metaDataItems.size(); i++) {

				// If system has not been started, look for system start
				if (!systemStarted && md.metaDataItems[i].code == 'S' && md.metaDataItems[i].descriptor == "begin") {
					systemStarted = true;
				}
				else if (!systemStarted) {
					throw std::logic_error("Cannot execute meta-data codes before system has been started; Check Meta-Data file.");
				}

				// Application handling
				else if (md.metaDataItems[i].code == 'A') {
					if (!readingApplication && md.metaDataItems[i].descriptor == "begin") {
						// New process!
						ProcessControlBlock process(processCount);
						processQueue.push_back(process);
						readingApplication = true;
					}
					else if (readingApplication && md.metaDataItems[i].descriptor == "begin") {
						throw std::logic_error("Cannot process Meta-Data codes for new application before previous application has finished; Check Meta-Data file.");
					}
					// Handle application code 
					bool applicationOK = handleApplicationCode(md.metaDataItems[i]);
					if (!applicationOK)
						throw std::logic_error("Error in handling an application-type Meta-Data code block; Check Meta-Data file.");
				}

				// S{finish}0;
				else if (md.metaDataItems[i].code == 'S' && md.metaDataItems[i].descriptor == "finish") {
					systemStarted = false;
					systemExited = true;
				}

				// Adding operations to an open application
				else {
					addOperation(processQueue[processCount], md.metaDataItems[i]);
				}
			}
		}

		else {
			throw std::logic_error("Operating system cannot process Meta-Data before Meta-Data is initialized. Failure initializing Meta-Data.");
		}
	}
	// Set meta-data processed!
	metaDataProcessed = true;
}

/**	Handle Application Code
*	\n Handle a meta-data code block which deals with Applications.
*	@param md is the meta-data code block being processed
*	@throw Error caused by application code calling for application to end, but one hasn't began.
*/
bool OperatingSystem::handleApplicationCode(MetaDataItem md) throw (std::logic_error){
	if (!readingApplication && md.descriptor == "begin") {
		return true;
	}
	else if (readingApplication && md.descriptor == "begin") {
		return true;
	}
	else if (readingApplication && md.descriptor == "finish") {
		// Process complete
		processCount++;
		readingApplication = false;
	}
	else if (!readingApplication && md.descriptor == "finish") {
		throw std::logic_error("Cannot finish an application before one begins; Check Meta-Data file.");
	}
	else {
		return false;
	}
	return true;
}

/**	Add Operation
*	\n Adds an operation to a process. Creates an operation based on the meta-data code block, then passes it to the process.
*	@param process which is taking the operation, passed by reference.
*	@param newOp is a meta-data code block which is to be translated into an operation.
*/
void OperatingSystem::addOperation(ProcessControlBlock &process, MetaDataItem newOp) {
	Operation tempOp(newOp.code, newOp.descriptor, newOp.timeVal);
	process.addOperation(tempOp);
}

/** Run Simulation
*	\n Run the simulation of the Oprerating System. Run a set of simulated processes.
*	@pre the queue of processes must be initialized with a set of process(es) which are also initialized.
*	@throw Cannot run the simulation before the meta-data has been processed
*/
void OperatingSystem::runSimulation() throw (std::logic_error){
	if (metaDataProcessed) {
		// Start timestamp timer
		logger.initializeLogSettings();

		// Log: (ts) Simulator Program Starting
		logger.writeWithTimestamp("Simulator program starting");

		// Loop through processes
		for (unsigned int i = 0; i < processQueue.size(); i++) {
			// Log: (ts) OS: Preparing Process (i)
			logger.writeWithTimestamp("OS: preparing process " + std::to_string(i+1));

			setReady(processQueue[i]);

			// Log: (ts) OS: Starting Process (i)
			logger.writeWithTimestamp("OS: starting process " + std::to_string(i+1));

			setRunning(processQueue[i]);

			// Run Process
			bool processOK = false;
			processOK = processQueue[i].run();
			if (!processOK) {
				std::cout << i << " th " << std::endl;
				throw std::logic_error("Process has failed to execute successfully.");
			}

			// Log: (ts) OS: Removing Process (i)
			logger.writeWithTimestamp("OS: removing process " + std::to_string(i+1));
			setExit(processQueue[i]);
		}

		// Log: (ts) Simulator Program Ending
		logger.writeWithTimestamp("Simulator program ending");
	}

	else {
		throw std::logic_error("Cannot run simulation before meta-data has been properly processed.");
	}
}

/** Set Ready
*	\n Sets a process' status to READY
*	@param process to be changed
*	@throw there are rules for how a process can move from one state to another; this function tried to break one of them.
*/
void OperatingSystem::setReady(ProcessControlBlock process) throw (std::logic_error){
	// Can move to READY from START, RUNNING, or WAITING
	if (process.getState() == ProcessControlBlock::START ||
			process.getState() == ProcessControlBlock::WAITING ||
				process.getState() == ProcessControlBlock::RUNNING) {
		process.changeState(ProcessControlBlock::READY);
	}
	else {
		throw std::logic_error("Invalid process state conversion to READY.");
	}
}

/** Set Running
*	\n Sets a process' status to RUNNING
*	@param process to be changed
*	@throw there are rules for how a process can move from one state to another; this function tried to break one of them.
*/
void OperatingSystem::setRunning(ProcessControlBlock process) throw (std::logic_error){
	// Can move to RUNNING from READY, only
	//if (process.getState() == ProcessControlBlock::READY) {
		process.changeState(ProcessControlBlock::RUNNING);
	//}
	//else {
	//	throw std::logic_error("Invalid process state conversion to RUNNING.");
	//}
}

/** Set Waiting
*	\n Sets a process' status to WAITING
*	@param process to be changed
*	@throw there are rules for how a process can move from one state to another; this function tried to break one of them.
*/
void OperatingSystem::setWaiting(ProcessControlBlock process) throw (std::logic_error){
	// Can move to WAITING from RUNNING, only
	//if (process.getState() == ProcessControlBlock::RUNNING) {
		process.changeState(ProcessControlBlock::WAITING);
	//}
	//else {
	//	throw std::logic_error("Invalid process state conversion to WAITING.");
	//}
}

/** Set Exit
*	\n Sets a process' status to EXIT
*	@param process to be changed
*	@throw there are rules for how a process can move from one state to another; this function tried to break one of them.
*/
void OperatingSystem::setExit(ProcessControlBlock process) throw (std::logic_error){
	// Can move to EXIT from RUNNING, only
	//if (process.getState() == ProcessControlBlock::RUNNING) {
		process.changeState(ProcessControlBlock::EXIT);
	//}
	//else {
	//	throw std::logic_error("Invalid process state conversion to EXIT.");
	//}
}


//
// MetaData Class Function Implementations ////////////////
//

/** @brief This class was declared within the OperatingSystem class because the OS could not do anything without it, and 
*			meta-data does not need to be used anywhere else enough for it to stand on it's own as a Class.
*/

/** Default Constructor
*	\n Creates an uninitialized MetaData object.
*/
OperatingSystem::MetaData::MetaData() {
	metaDataInitialized = false;
	// Initialize the Meta-Data object with information in Config file
	MetaDataInit(conf.metaDataFilename);
}

/** Get Initialized Status
*	\n Accessor for metaDataInitialized
*	@return true if initialized, false otherwise
*/
bool OperatingSystem::MetaData::getInitializedStatus() const{
	return metaDataInitialized;
}

/** Meta Data Init.
*	\n Initializes all Meta-Data via meta-data file path specified in configuration data.
*	Opens the file containing meta-data information, then reads in the data from the specifically formatted file.
*	@pre Configuration data must be read and processed.
*	@param fin is an input filestream object which holds meta-data information
*	@param metaDataFilename the name of the file which holds meta-data information
*	@throw Error in opening meta-data file
*/
void OperatingSystem::MetaData::MetaDataInit(std::string metaDataFilename) throw(std::logic_error) {
	std::ifstream fin;

	fin.open(metaDataFilename);		// open meta data file
	if (!fin.good()) {
		throw std::logic_error("Meta-Data file path unresolved; check config file.");		// If file opening fails, throw error
	}

	// Read from file
	ReadDescriptor(fin, '\n');								// Read in "Start Program Meta-Data Code:"
	while (fin.peek() != '.') {								// While there is Meta-data to read (delimited by '.')

		if ((fin >> std::ws).peek() == '{')					// Check for meta-data code
			throw std::logic_error("Meta-Data code missing from meta-data block; check meta-data file.");

		metaDataItem.code = ReadCode(fin);					// Read in meta data code
		fin.get();											// eat '{'

		if ((fin >> std::ws).peek() == '}')					// Check for meta-data descriptor
			throw std::logic_error("Meta-Data descriptor missing from meta-data block; check meta-data file.");

		metaDataItem.descriptor = ReadDescriptor(fin, '}');	// Read in descriptor

		if ((fin >> std::ws).peek() == ';')					// Check for meta-data cycle time
			throw std::logic_error("Meta-Data cycle time missing from meta-data block; check meta-data file.");

		fin >> metaDataItem.timeVal;						// Read in time
		if (fin.peek() != '.') {
			fin.get();										// eat ';'
		}
		metaDataItems.push_back(metaDataItem);				// Load meta data block into Meta-Data container
	}
	fin.get();												// eat '.'
	fin.get();												// eat '\n'
	ReadDescriptor(fin, '.');								// Read in "End Program Meta-Data Code"

	// Initialization complete, close file
	fin.close();

	metaDataInitialized =  true;
}

/** Read Code
*	\n Reads in a meta-data code, then tests it for validity.
*	@param fin is the input filestream object from which the code is read
*	@return A verified meta-data code
*	@throw Error caused by invalid code
*/
char OperatingSystem::MetaData::ReadCode(std::ifstream & fin) throw(std::logic_error) {
	char read;

	(fin >> std::ws).get(read);

	for (unsigned int i = 0; i < sizeof(codes); i++) {
		if (read == codes[i]) {
			return read;
		}
	}

	throw std::logic_error("Meta-Data code read error; check Meta-Data file.");
}

/** Read Descriptor
*	\n Reads in a meta-data descriptor, then tests it for validity.
*	@param fin is the input filestream object from which the descriptor is read
*	@return A verified meta-data descriptor
*	@throw Error caused by invalid descriptor
*/
std::string OperatingSystem::MetaData::ReadDescriptor(std::ifstream & fin, char delimiter) throw(std::logic_error) {
	std::string read;

	std::getline(fin, read, delimiter);

	for (unsigned int i = 0; i < (sizeof(descriptors) / sizeof(descriptors[0])); i++) {		// Loop for each element in array
		if (read == descriptors[i]) {
			return read;
		}
	}

	throw std::logic_error("Meta-Data descriptor read error; check Meta-Data file.");
}

/** Calculate Run Time
*	\n Calculates the run time of a meta-data process by looking up the process in the configuration information vector
*	by name, then multiplying it's matching time value by the number of cycles denoted in the meta-data file.
*	@pre Configuration data must be read and processed
*	@param config is the configuration data stored in a vector of pairs, where the pair contains the process name and the matching timing per cycle.
*	@param metaDescriptor is the descriptor being conducted
*	@param metaTime is the number of cycles the process is being ran for
*	@return The run time in milliseconds for the meta-data operation
*	@throw Error finding timing for specified meta-data operation
*	@note AS OF LATEST VERSION, NEED TO SIMPLIFY THIS FUNCTION IN LIGHT OF ADDITION OF EXTERN CONFIG DECLARATION**************
*/
int OperatingSystem::MetaData::CalculateRunTime(char metaCode, std::string metaDescriptor, int metaTime) const {
	return metaTime * conf.GetOperationTime(metaCode, metaDescriptor);
}

/**	Show Meta Data			(FOR DEBUGGING, NOT GRADING)
*	\n Outputs meta-data to either the console, a specified file, or both
*	@pre Both configuration and meta-data must be read and processed
*	@param fout is the output file object to which the meta-data will be output
*	@param configInfo is the configuration data stored in a vector of pairs, where the pair contains the process name and the matching timing per cycle
*	@param logSetting is a string which denotes to where the data will be output

void OperatingSystem::MetaData::ShowMetaData(std::ofstream & fout, std::string logSetting) const {
	// If printing to monitor, or both monitor and file:
	if ((logSetting == "Monitor") || (logSetting == "Both")) {
		std::cout << "Meta-Data Metrics" << std::endl;
		for (unsigned int i = 0; i < metaDataItems.size(); i++) {
			if (metaDataItems[i].timeVal > 0) {
				std::cout << metaDataItems[i].code << '{' << metaDataItems[i].descriptor << "}" << metaDataItems[i].timeVal << " - "
					<< CalculateRunTime(metaDataItems[i].code, metaDataItems[i].descriptor, metaDataItems[i].timeVal) << " ms";
				if (i != metaDataItems.size() - 1) {
					std::cout << std::endl;
				}
			}
		}
		if (logSetting == "Both") {
			fout << "Meta-Data Metrics" << std::endl;
			for (unsigned int i = 0; i < metaDataItems.size(); i++) {
				if (metaDataItems[i].timeVal > 0) {
					fout << metaDataItems[i].code << '{' << metaDataItems[i].descriptor << "}" << metaDataItems[i].timeVal << " - "
						<< CalculateRunTime(metaDataItems[i].code, metaDataItems[i].descriptor, metaDataItems[i].timeVal) << " ms";
					if (i != metaDataItems.size() - 1) {
						fout << std::endl;
					}
				}
			}
		}


	}
	// otherwise, only output to file
	else {
		fout << "Meta-Data Metrics" << std::endl;
		for (unsigned int i = 0; i < metaDataItems.size(); i++) {
			if (metaDataItems[i].timeVal > 0) {
				fout << metaDataItems[i].code << '{' << metaDataItems[i].descriptor << "}" << metaDataItems[i].timeVal << " - "
					<< CalculateRunTime(metaDataItems[i].code, metaDataItems[i].descriptor, metaDataItems[i].timeVal) << " ms";
				if (i != metaDataItems.size() - 1) {
					fout << std::endl;
				}
			}
		}
	}
}*/
