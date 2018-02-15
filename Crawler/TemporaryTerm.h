#ifndef TEMPTERM_H
#define TEMPTERM_H

#include <iostream>
#include "List.h"

/*
	this class is used to temporary store terms in documents before putting them in the main tree
*/
class TemporaryTerm
{
public:
	std::string term; // term name
	std::string fileName; // file name the term is in
	int id = 0; // id of the file
	int termCount = 0;
	List<int> positions; // positions of the term
	TemporaryTerm(); // default constructor
	bool operator==(TemporaryTerm data);
};

#endif