#ifndef TERM_H
#define TERM_H

#include <iostream>
#include "List.h"
#include "TermInFile.h"

class Term
{
public:
	Term();
	std::string term; // the word that its indexed
	List<TermInFile> files; //in which files
	Term *right;
	Term *left;
};
#endif 