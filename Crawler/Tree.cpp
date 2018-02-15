#include "Tree.h"
#include "Main.h"
#include <string>

Tree::Tree()
{
	rootTerm = NULL;
}

void Tree::insert(Term *&root, TemporaryTerm *addTerm)
{
	if (root == NULL) // if current node is null insert there
	{
		Term *term = new Term();
		term->term = addTerm->term;

		TermInFile *termInFile = new TermInFile();
		termInFile->fileName = addTerm->fileName;
		termInFile->positions = addTerm->positions;
		termInFile->totalCount = addTerm->termCount;
		termInFile->id = addTerm->id;

		term->files.insert(*termInFile);

		root = term;
	}
	else if (root->term.compare(addTerm->term) < 0) // if compare < 0 insert in left node
	{
		insert(root->left, addTerm);
	}
	else if (root->term.compare(addTerm->term) == 0) // if compare == 0 add the termInFile to the termInFile list in Term
	{
		TermInFile *termInFile = new TermInFile();
		termInFile->fileName = addTerm->fileName;
		termInFile->positions = addTerm->positions;
		termInFile->totalCount = addTerm->termCount;
		termInFile->id = addTerm->id;
		root->files.insert(*termInFile);
	}
	else // if compare > 0 insert in right node
	{
		insert(root->right, addTerm);
	}
}

/*
	search for a word in the tree
*/
void Tree::search(Term *&root,std::string searchString, List<TermInFile> *foundIn)
{
	if (root == NULL)
	{
		return;
	}

	if (root->term.compare(searchString) == 0) // if compare == 0 populate the result entry (foundIn) with the data of the Term
	{
		ListNode<TermInFile> *termInFile = root->files.startPtr;

		while (termInFile != NULL)
		{
			foundIn->insert(termInFile->data);
			termInFile = termInFile->nextPtr;
		}
	}
	else if (root->term.compare(searchString) < 0) // if compare < 0 search left node
	{
		search(root->left, searchString, foundIn);
	}
	else // if compare > 0 search right node
	{
		search(root->right, searchString, foundIn);
	}
}

/*
	search the whole tree for multiple results of a word
	this is used for the term* search
*/
void Tree::inOrderSearch(Term *&root, std::string searchString, List<TermInFile> *foundIn, List<TermInFile> *result)
{
	if (root != NULL)
	{
		inOrderSearch(root->left, searchString, foundIn, result); //recursion
		if (root->term.substr(0, searchString.length()) == searchString)
		{
			Main::getTermUnion(root->files, *foundIn, result); //find all the results of all words that match term* in documents
		}
		inOrderSearch(root->right, searchString, foundIn, result); //recursion
	}
}

/*
	this function is used to set the search words and their operations with other words
*/
void Tree::setSearchWords(List<SearchWord> *searchWords, std::string str)
{
	std::string previousWord = ""; //get the previous word
	std::string currentWord = ""; //get the current word
	bool notFlag = false; //flag for not operation
	int index = 0; //current char index of the search string
	while (true)
	{
		if (str[index] == '\0') // check if the string ended
		{
			if (previousWord.length() > 0) // if there is a previous word and a current word then theres a combination of two words with a 'with' operation
			{
				SearchWord *searchWord = new SearchWord();
				searchWord->operationNext = "with";
				searchWord->word = previousWord;
				if (previousWord.at(previousWord.length() - 1) == '*') //check if the word ends in *
				{
					searchWord->isFull = false; //set the flag for the * search
					previousWord = previousWord.substr(0, previousWord.length() - 1); //remove the *
				}
				searchWords->insert(*searchWord);
			}
			SearchWord *searchWord = new SearchWord();
			if (currentWord.at(currentWord.length() - 1) == '*')
			{
				searchWord->isFull = false;
				currentWord = currentWord.substr(0, currentWord.length() - 1);
			}
			searchWord->word = currentWord;
			searchWord->notFlag = notFlag;

			searchWords->insert(*searchWord);
			currentWord = "";
			break;
		}
		if (str[index] == ' ') // split the word if there is a ' '
		{
			if (currentWord == "and" || currentWord == "or") // if the current words are and || or, add the previous word with 'and' or 'or' operation
			{
				SearchWord *searchWord = new SearchWord();
				if (previousWord.at(previousWord.length() - 1) == '*')
				{
					searchWord->isFull = false;
					previousWord = previousWord.substr(0, previousWord.length() - 1);
				}

				searchWord->operationNext = currentWord; // set the operation word
				searchWord->word = previousWord; // set the word
				searchWord->notFlag = notFlag; // set the flag
					
				searchWords->insert(*searchWord); // insert in the list
				previousWord = "";
				notFlag = false;
			}
			else if (currentWord == "not") // set flag if there is a not operation in the search
			{
				notFlag = true;
			}
			else if (previousWord.length() > 0) // if there is no operation then choose operation with between both words
			{
				SearchWord *searchWord = new SearchWord();
				searchWord->operationNext = "with";
				searchWord->word = previousWord;
				searchWord->notFlag = notFlag;

				if (previousWord.at(previousWord.length() - 1) == '*')
				{
					searchWord->isFull = false;
					previousWord = previousWord.substr(0, previousWord.length() - 1);
				}

				searchWords->insert(*searchWord);

				notFlag = false;
			}
			if (currentWord.compare("and") != 0 && currentWord.compare("or") != 0 && currentWord.compare("not") != 0)
			{
				previousWord = currentWord; // if the current word is not any of the operations set the previous word
			}
			currentWord = "";
			index++;
		}
		currentWord += str[index];
		index++;
	}
}

/*
	this function is used to find the words in the documents
*/
void Tree::findSearchWordsinDocs(List<SearchWord> *searchWords)
{
	ListNode<SearchWord> *searchWord = searchWords->startPtr; // set an iterator for the list

	while (searchWord != NULL) //iterate until iterator is null
	{
		if (searchWord->data.isFull) // if the flag is true then there is no * search
		{
			Main::tree.search(Main::tree.rootTerm, searchWord->data.word, &searchWord->data.foundInDocs);
		}
		else // else we use the in order search to find all term*
		{
			List<TermInFile> temp;
			Main::tree.inOrderSearch(Main::tree.rootTerm, searchWord->data.word, &temp, &searchWord->data.foundInDocs);
		}
		if (searchWord->data.notFlag) // if the not flag is set get all the pages that this word is not in
		{
			List<TermInFile> temp; // create a temporary list to hold the result
			ListNode<IndexedPage> *indexPageNode = Main::indexedPages.startPtr; // iterator for the indexed pages
			ListNode<TermInFile> *termInFile = searchWord->data.foundInDocs.startPtr; // iterator for the term pages
			if (termInFile == NULL) // if the terms pages list is empty insert all the indexed pages in its list
			{
				while (indexPageNode != NULL)
				{
					TermInFile *tempTerm = new TermInFile();
					tempTerm->fileName = indexPageNode->data.pageName;
					temp.insert(*tempTerm);
					indexPageNode = indexPageNode->nextPtr;
				}
			}
			else //if its not empty, iterate through both lists and find all the missing pages from the terms pages list
			{
				while (indexPageNode != NULL) // iterate through indexed pages
				{
					while (termInFile != NULL) // iterate through terms pages
					{
						if (indexPageNode->data.pageName != termInFile->data.fileName) // if they are different insert in the temporary list
						{
							TermInFile *tempTerm = new TermInFile();
							tempTerm->fileName = indexPageNode->data.pageName;
							temp.insert(*tempTerm);
						}
						termInFile = termInFile->nextPtr;
					}
					indexPageNode = indexPageNode->nextPtr;
				}
			}
			searchWord->data.foundInDocs.clear(); // clear the search words list
			searchWord->data.foundInDocs = temp; // and add the new list
		}

		searchWord = searchWord->nextPtr;
	}
}

/*
	this function is used for setting the final search result
*/
void Tree::setSearchWordsResult()
{

	ListNode<SearchWord> *searchWord = Main::tree.searchWords.startPtr; // search words list iterator

	while (searchWord->nextPtr != NULL)
	{
		List<TermInFile> temp; // temporary list to hold data
		if (searchWord->nextPtr->data.operationNext == "with") // check if the next list operation is with, because with has priority over other operations
		{
			Main::getTermWith(searchWord->nextPtr->data.foundInDocs, searchWord->nextPtr->nextPtr->data.foundInDocs, &temp); // find the documents that both terms are side by side
			searchWord->nextPtr->data.word += " " + searchWord->nextPtr->data.operationNext; // concatinate the operation
			searchWord->nextPtr->data.word += " " + searchWord->nextPtr->nextPtr->data.word; // concatinate the next word
			searchWord->nextPtr->data.foundInDocs.clear(); // clear list contents
			searchWord->nextPtr->data.foundInDocs = temp; // set the new list
			searchWord->nextPtr->data.operationNext = searchWord->nextPtr->nextPtr->data.operationNext; // set the operation of the next search word

			Main::tree.searchWords.pop(searchWord->nextPtr->nextPtr->data); // remove the next search word
		}
		else
		{
			if (searchWord->data.operationNext == "with")
			{
				Main::getTermWith(searchWord->data.foundInDocs, searchWord->nextPtr->data.foundInDocs, &temp); // find both words documents with 'with' operation
			}
			else if (searchWord->data.operationNext == "and")
			{
				Main::getTermIntersection(searchWord->data.foundInDocs, searchWord->nextPtr->data.foundInDocs, &temp); // find both words documents with 'and' operation
			}
			else if (searchWord->data.operationNext == "or")
			{
				Main::getTermUnion(searchWord->data.foundInDocs, searchWord->nextPtr->data.foundInDocs, &temp);// find both words documents with 'or' operation
			}
			searchWord->data.word += " " + searchWord->data.operationNext;// concatinate the operation
			searchWord->data.word += " " + searchWord->nextPtr->data.word;// concatinate the next word
			searchWord->data.foundInDocs.clear();// clear list contents
			searchWord->data.foundInDocs = temp;// set the new list
			searchWord->data.operationNext = searchWord->nextPtr->data.operationNext;// set the operation of the next search word

			Main::tree.searchWords.pop(searchWord->nextPtr->data);// remove the next search word
		}
	}
}