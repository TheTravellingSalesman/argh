/**
*	@file MetaData.h
*	@author Brian Marks
*	@version 1.2
*	@details Class declaration for the storing and handling of Meta-Data file data
*	@note 1.2 update: Adjusted ReadDescriptor to be compatible with new meta-data file format/descriptor options.
*	@date Monday, Feb. 26, 2018
*/

//
// Compiler Guards ////////////////////////
//
#ifndef METADATA_H
#define METADATA_H

//
// Header Files ///////////////////////////
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Config.h"

extern Config conf;

//
// Struct Declarations ////////////////////
//
struct MetaDataItem {
	char code;
	std::string descriptor;
	int timeVal;
};

//
// Class Function Declarations ////////////
//
class MetaData {
public:
	// Constructors
	MetaData();			// Default

	// Sets
	void MetaDataInit(std::ifstream& fin, std::string metaDataFilename) throw(std::logic_error);

	// Additional Functions
	char ReadCode(std::ifstream& fin) throw(std::logic_error);
	std::string ReadDescriptor(std::ifstream& fin, char delimiter) throw(std::logic_error);
	int CalculateRunTime(std::vector< std::pair<std::string, int> > config, char metaCode, std::string metaDescriptor, int metaTime) const throw(std::logic_error);
	void ShowMetaData(std::ofstream & fout, std::vector< std::pair<std::string, int> > configInfo, std::string loggingSetting) const;

	// Meta-Data code block vector (queue)
	std::vector<MetaDataItem> metaDataItems;

private:
	// Error handling data items
	char codes[6] = { 'S', 'A', 'P', 'I', 'O', 'M' };
	std::string descriptors[12] = { "begin", "finish", "hard drive",
		"keyboard", "scanner", "monitor",
		"run", "allocate", "projector",
		"block", "Start Program Meta-Data Code:",
		"End Program Meta-Data Code" };

	// Meta-Data container
	MetaDataItem metaDataItem;
};

#endif // !METADATA_H