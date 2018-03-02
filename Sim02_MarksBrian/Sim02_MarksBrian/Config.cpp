/**
*	@file Config.cpp
*	@author Brian Marks
*	@version 1.2
*	@details Class definition for the storing and handling of Configuration file data
*	@note 1.2 update: Adjusted ReadKey to be compatible with new configuration file format/descriptor options.
*	@date Monday, Feb. 26, 2018
*/

//
// Header Files /////////////////////////////
//
#include "Config.h"

//
// Class Function Definitions /////////////////
//

/** Default Constructor
*	\n Creates an empty Config object
*/
Config::Config() {
	version = " ";
	metaDataFilename = " ";
	logPath = " ";
	logSetting = " ";
}

/** Open Log Path
*	\n Opens the file which will store the Log information for both the config data and meta-data.
*	If the file is already open, nothing happens.
*	@param logFile is an output filestream object, which is passed by reference from Sim01.cpp
*	@throw Error opening file
*/
void Config::OpenLogPath(std::ofstream& logFile) throw (std::logic_error) {
	if (!logFile.is_open()) {		// check if log file is already open
		logFile.open(logPath);		// open log file
		if (!logFile.good()) {
			throw std::logic_error("Log file path unresolved; check config file.");		// If file opening fails, throw error
		}
	}
}
/** Get Log Setting
*	\n A getter function for the log setting denoted by the config file
*	@return logSetting is a string denoting "Monitor", "Both", or "File"
*/
std::string Config::GetLogSetting() const {
	return logSetting;
}

/** Get Operation Time
*	\n Looks up the run time of a meta-data operation in the configuration information vector
*	by name, then returns the milliseconds required per cycle for that operation.
*	@pre Configuration data must be read and processed
*	@param operationName is the name of the key being looked up in the configInfo vector of pairs<key, timing>.
*	@param metaDescriptor is the descriptor being conducted
*	@param metaTime is the number of cycles the process is being ran for
*	@return The milliseconds/cycle for a specified meta-data operation.
*	@throw Error finding timing for specified meta-data operation; check config file.
*/
int Config::GetOperationTime(char metaCode, std::string metaDescriptor ) const throw(std::logic_error){
	// If calculating a processor based process
	if (metaCode == 'P') {
		for (unsigned int i = 0; i < configInfo.size(); i++) {
			if (configInfo[i].first == "processor") {		// look up time in config vector
				return configInfo[i].second;
			}
		}
	}
	// If calculating a memory based process
	else if (metaCode == 'M') {
		for (unsigned int i = 0; i < configInfo.size(); i++) {
			if (configInfo[i].first == "memory") {			// look up time in config vector
				return configInfo[i].second;
			}
		}
	}
	// For all other processes
	else {
		for (unsigned int i = 0; i < configInfo.size(); i++) {	// look up descriptor in config vector
			if (configInfo[i].first == metaDescriptor) {		// look up time in config vector
				return configInfo[i].second;
			}
		}
	}

	throw std::logic_error("Config file does not contain timing information for a Meta-Data operation; check Config file.");
}

long Config::GetKbytesAvailable() const throw(std::logic_error){
	long unsigned int element = (configInfo.size() - 4);
	if (configInfo[element].first[15] == 'k') {
		return configInfo[element].second;
	}
	else if (configInfo[element].first[15] == 'M') {
		return (configInfo[element].second * 1000);
	}
	else if (configInfo[element].first[15] == 'G') {
		return (configInfo[element].second * 1000000);
	}
	else {
		throw std::logic_error("Could not determine system memory size in kbytes, Mbytes, or Gbytes; Check config file.");
	}
}

/** Config Init.
*	\n Initializes all configuration data by reading from a specified file from the command line.
*	Data read from file is checked for accuracy, then pertinent data is stored in a vector of pairs.
*	Configuration data file MUST include data for the "Log File Path:"; NULL or otherwise.
*	@param fileIn is a c-style string denoting the name of the configuration data input file
*	this is passed by argv[1] from Sim01.cpp
*	@throw Error opening file
*/
void Config::ConfigInit(char* fileIn) throw (std::logic_error) {
	// Declare vars
	std::string key;					// Key for configInfo
	std::string loggingType;			// String determining logSetting
	int value;							// Value for configInfo
	std::ifstream fin;					// Open input file
	fin.open(fileIn);

	if (!fin.good()) {
		throw std::logic_error("Unresolved config file name; check command line parameter.");
	}

	// Read from file
	ReadKey(fin, '\n');					// read in "Start Simulator Config. File"
	ReadKey(fin, ' ');					// read in "Version/Phase:"
	fin >> version;						// read in version number
	fin.get();							// eat newline character
	ReadKey(fin, ':');					// read in "File Path:"
	fin.get();							// eat space
	std::getline(fin, metaDataFilename);			// read in MetaData file name
	
	// Make sure meta-data file is of .mdf extention
	if (metaDataFilename.substr(metaDataFilename.size() - 4) != ".mdf")
		throw std::logic_error("Meta-data file specified has wrong extention (.mdf); check configuration file.");

	while (fin.peek() != 'L') {				// while next item isn't "Log:"
		key = ReadKey(fin, ':');			// read in process name (first for configInfo)
		fin >> value;						// read in timing value (second for configInfo)
		fin.get();							// eat newline character
		configInfo.push_back(std::make_pair(key, value));			// store key/value pair in vector
	}

	ReadKey(fin, ' ');					// read in "Log:"
	std::getline(fin, loggingType);		// read in Logging configuration
	SetLogSetting(loggingType);			// set logSetting
	ReadKey(fin, ':');					// read in "Log File Path:"
	fin.get();							// eat space
	std::getline(fin, logPath);			// read in log path
	ReadKey(fin, 'e');					// read in "End Simulator Configuration File"
}

/** Set Log Setting
*	\n Checks the log setting denoted by the configuration file for validity, then shortens the name to simpler terms.
*	@param type is a string denoting the log setting specified by the config file.
*	@throw Error caused by invalid or misspelled log setting
*/
void Config::SetLogSetting(std::string type) throw(std::logic_error) {
	if (type == "Log to Monitor") {
		logSetting = "Monitor";
	}

	else if (type == "Log to File") {
		logSetting = "File";
	}

	else if (type == "Log to Both") {
		logSetting = "Both";
	}
	else {
		throw std::logic_error("The logging type is invalid; check config file.");
	}
}

/**	Read Key
*	\n Checks either a line or timing descriptor in the config file for accuracy in formatting and spelling,
*	then reformats it for use in output.
*	Updated to include the System memory specification in the config file.
*	@param fin is an input filestream object passed by reference
*	@param delimiter is the character at which the function will stop reading from the file
*	@return A reformatted string which holds the timing descriptor or debugging information
*	@throw Error caused by spelling/formatting error in config file line or timing descriptor
*/
std::string Config::ReadKey(std::ifstream& fin, const char delimiter) throw(std::logic_error) {
	std::string key;					// Will hold the description for the following value in the config file

	std::getline(fin, key, delimiter);	// Get line from the config file

	for (unsigned int i = 0; i < (sizeof(configReads) / sizeof(configReads[0])); i++) {
		if (key == configReads[i]) {
			switch (i) {				// Depending on the process name, shorten key to single term
			case 3:
				key = "Monitor";
				break;
			case 4:
				key = "Processor";
				break;
			case 5:
				key = "Scanner";
				break;
			case 6:
				key = "Hard drive";
				break;
			case 7:
				key = "Keyboard";
				break;
			case 8:
				key = "Memory";
				break;
			case 9:
				key = "Projector";
				break;
			case 10:
				key = "System memory";
				break;
			default:
				key = "Error in config key";
			}

			return key;					// Format/Spelling is accurate, return good key
		}
	}

	throw std::logic_error("Format/Spelling inaccurate; check config file.");		// This key was not found in list of possible config reads, throw error
}

/** Show Config
*	\n Outputs the configuration data to either the console, a specified file, or both.
*	This is the last configuration data function which uses the descriptors, so this reformats that data for use in meta-data functions.
*	@pre Configuration data must be loaded
*	@param fout is the file to which the configuration data will be output
*/
void Config::ShowConfig(std::ofstream& fout) {
	OpenLogPath(fout);		// Open the log file (if not already open)

							// If printing to monitor, or both monitor and file:
	if ((logSetting == "Monitor") || (logSetting == "Both")) {
		std::cout << "Configuration File Data" << std::endl;				// Print title

		for (unsigned int i = 0; i < configInfo.size(); i++) {				// Print processes and their timings
			std::cout << configInfo[i].first << " = "
				<< configInfo[i].second << " ms/cycle" << std::endl;
		}

		std::cout << "Logged to: monitor";

		if (logSetting == "Both") {											// Output to file as well if logSetting is Both
			std::cout << " and " << logPath;

			fout << "Configuration File Data" << std::endl;					// Print title

			for (unsigned int i = 0; i < configInfo.size(); i++) {			// Print processes and their timings
				fout << configInfo[i].first << " = "
					<< configInfo[i].second << " ms/cycle" << std::endl;
			}
			fout << "Logged to: monitor and " << logPath << std::endl << std::endl;
		}
		std::cout << std::endl << std::endl;
	}

	// otherwise, only output to file
	else {
		fout << "Configuration File Data" << std::endl;						// Print title

		for (unsigned int i = 0; i < configInfo.size(); i++) {				// Print processes and their timings
			fout << configInfo[i].first << " = "
				<< configInfo[i].second << " ms/cycle" << std::endl;
		}
		fout << "Logged to: " << logPath << std::endl << std::endl;
	}

	// Done with configInfo for config purposes; Change format of configInfo to fit meta-data
	for (unsigned int i = 0; i < configInfo.size(); i++) {
		configInfo[i].first[0] = tolower(configInfo[i].first[0]);		// Make first letter of each descriptor lower-case
	}
}
