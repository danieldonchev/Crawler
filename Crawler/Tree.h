#ifndef TREE_H
#define TREE_H

#include <iostream>
#include "Term.h"
#include "TemporaryTerm.h"
#include "SearchWord.h"

class Tree
{
public:
	Tree();
	void insert(Term *&root, TemporaryTerm *addTerm); //insert term in tree
	void search(Term *&root, std::string searchString, List<TermInFile> *foundIn); //search item in tree
	void inOrderSearch(Term *&root, std::string searchString, List<TermInFile> *foundIn, List<TermInFile> *result); //search all items in the tree
	void setSearchWords(List<SearchWord> *searchWords, std::string str); // set search words and their operations
	void findSearchWordsinDocs(List<SearchWord> *); // find words in the documents
	void setSearchWordsResult(); // use operations
	Term *rootTerm; 
	List<SearchWord> searchWords; //words to search in the tree
};


#endif 