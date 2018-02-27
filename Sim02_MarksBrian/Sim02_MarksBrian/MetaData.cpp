/**
*	@file MetaData.cpp
*	@author Brian Marks
*	@version 1.2
*	@details Class definition for the storing and handling of Meta-Data file data
*	@note 1.2 update: Adjusted ReadDescriptor to be compatible with new meta-data file format/descriptor options.
*	@date Monday, Feb. 26, 2018
*/

//
// Header Files ///////////////////////////
//
#include "MetaData.h"

/** Default Constructor
*	\n Creates an uninitialized MetaData object.
*/
MetaData::MetaData() {
	metaDataItem.code = 'X';
	metaDataItem.descriptor = " ";
	metaDataItem.timeVal = 0.0;
}

/** Meta Data Init.
*	\n Initializes all Meta-Data via meta-data file path specified in configuration data.
*	Opens the file containing meta-data information, then reads in the data from the specifically formatted file.
*	@pre Configuration data must be read and processed.
*	@param fin is an input filestream object which holds meta-data information
*	@param metaDataFilename the name of the file which holds meta-data information
*	@throw Error in opening meta-data file
*/
void MetaData::MetaDataInit(std::ifstream& fin, std::string metaDataFilename) throw(std::logic_error) {
	if (!fin.is_open()) {		// check if meta data file is already open
		fin.open(metaDataFilename);		// open meta data file
		if (!fin.good()) {
			throw std::logic_error("Meta-Data file path unresolved; check config file.");		// If file opening fails, throw error
		}
	}

	// Read from file
	ReadDescriptor(fin, '\n');								// Read in "Start Program Meta-Data Code:"
	while (fin.peek() != '.') {								// While there is Meta-data to read (delimited by '.')
		metaDataItem.code = ReadCode(fin);					// Read in meta data code
		fin.get();											// eat '{'
		metaDataItem.descriptor = ReadDescriptor(fin, '}');	// Read in descriptor
		fin >> metaDataItem.timeVal;						// Read in time
		if (fin.peek() != '.') {
			fin.get();										// eat ';'
			fin.get();										// eat ' '
		}
		metaDataItems.push_back(metaDataItem);				// Load meta data block into Meta-Data container
	}
	fin.get();												// eat '.'
	fin.get();												// eat '\n'
	ReadDescriptor(fin, '.');								// Read in "End Program Meta-Data Code"
}

/** Read Code
*	\n Reads in a meta-data code, then tests it for validity.
*	@param fin is the input filestream object from which the code is read
*	@return A verified meta-data code
*	@throw Error caused by invalid code
*/
char MetaData::ReadCode(std::ifstream & fin) throw(std::logic_error) {
	char read;

	fin.get(read);

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
std::string MetaData::ReadDescriptor(std::ifstream & fin, char delimiter) throw(std::logic_error) {
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
*	@throw Error finding timing for specified meta-data process
*/
int MetaData::CalculateRunTime(std::vector< std::pair<std::string, int> > config, char metaCode, std::string metaDescriptor, int metaTime) const throw(std::logic_error) {
	// If calculating a processor based process
	if (metaCode == 'P') {
		for (unsigned int i = 0; i < config.size(); i++) {
			if (config[i].first == "processor") {		// look up time in config vector
				return ((config[i].second) * metaTime);
			}
		}
	}
	// If calculating a memory based process
	else if (metaCode == 'M') {
		for (unsigned int i = 0; i < config.size(); i++) {
			if (config[i].first == "memory") {			// look up time in config vector
				return ((config[i].second) * metaTime);
			}
		}
	}
	// For all other processes
	else {
		for (unsigned int i = 0; i < config.size(); i++) {	// look up descriptor in config vector
			if (config[i].first == metaDescriptor) {		// look up time in config vector
				return ((config[i].second) * metaTime);
			}
		}
	}

	throw std::logic_error("Config file does not contain timing information for a Meta-Data command; check Config and Meta-Data files");
}

/**	Show Meta Data
*	\n Outputs meta-data to either the console, a specified file, or both
*	@pre Both configuration and meta-data must be read and processed
*	@param fout is the output file object to which the meta-data will be output
*	@param configInfo is the configuration data stored in a vector of pairs, where the pair contains the process name and the matching timing per cycle
*	@param logSetting is a string which denotes to where the data will be output
*/
void MetaData::ShowMetaData(std::ofstream & fout, std::vector< std::pair<std::string, int> > configInfo, std::string logSetting) const {
	// If printing to monitor, or both monitor and file:
	if ((logSetting == "Monitor") || (logSetting == "Both")) {
		std::cout << "Meta-Data Metrics" << std::endl;
		for (unsigned int i = 0; i < metaDataItems.size(); i++) {
			if (metaDataItems[i].timeVal > 0) {
				std::cout << metaDataItems[i].code << '{' << metaDataItems[i].descriptor << "}" << metaDataItems[i].timeVal << " - "
					<< CalculateRunTime(configInfo, metaDataItems[i].code, metaDataItems[i].descriptor, metaDataItems[i].timeVal) << " ms";
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
						<< CalculateRunTime(configInfo, metaDataItems[i].code, metaDataItems[i].descriptor, metaDataItems[i].timeVal) << " ms";
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
					<< CalculateRunTime(configInfo, metaDataItems[i].code, metaDataItems[i].descriptor, metaDataItems[i].timeVal) << " ms";
				if (i != metaDataItems.size() - 1) {
					fout << std::endl;
				}
			}
		}
	}
}
