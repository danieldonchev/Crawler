#ifndef SEARCHWORDS_H
#define SEARCHWORDS_H


#include <iostream>
#include <string>
#include "List.h"
#include "TermInFile.h"

/*
	this class is used for the words that need to be searched in the documents
*/
class SearchWord
{
public:
	int priority = 0;
	bool notFlag = false; // flag if the word needs to be displayed in which documents it doesnt appear
	bool isFull = true; // flag for search with *
	std::string word;
	std::string operationNext; // the operation with the next word
	List<TermInFile> foundInDocs; // in which documents is the word found
	bool operator==(SearchWord data); // predefine operator
	friend std::ostream& operator<<(std::ostream &os, SearchWord data)
	{
		ListNode<TermInFile> *node = data.foundInDocs.startPtr;
		os << std::endl << std::endl;
		while (node != NULL)
		{
			os << node->data.fileName << std::endl;
			node = node->nextPtr;
		}
		return os;
	}
	SearchWord();
};


#endif