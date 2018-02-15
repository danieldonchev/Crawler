#ifndef TERMINFILE_H
#define TERMINFILE_H

#include <iostream>
#include "List.h"

/*
	this class is for the Terms and in which files are they and in which positions
*/

class TermInFile
{
public:
	int id = 0; //set the file id
	std::string fileName; // set the file name
	List<int> positions; // list of positions
	int totalCount;
	bool operator>(TermInFile data);
	bool operator==(TermInFile data);
};

#endif