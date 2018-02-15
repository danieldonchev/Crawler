#include "HttpClient.h"
#include "List.h"
#include "Tree.h"
#include "Main.h"
#include "SearchWord.h"

int main()
{
	Main::startIndexing("http://crawlertest.cs.tu-varna.bg/", 2);
	Main::indexedPages.sort();
	std::string searchString;


	std::cout << std::endl << std::endl;
	std::cout << "Page indexed";
	std::cout << std::endl << std::endl;
	std::cout << "Enter a search query:" << std::endl;
	std::cin >> searchString;
	Main::tree.setSearchWords(&Main::tree.searchWords, searchString);
	Main::tree.findSearchWordsinDocs(&Main::tree.searchWords);
	Main::tree.setSearchWordsResult();

	Main::tree.searchWords.print();


	return 0;
}

Tree Main::tree;
List<IndexedPage> Main::indexedPages;
List<Http> Main::Pages;
int Main::id = 0;

void Main::startIndexing(std::string url, int depth)
{
	Http http(url, NULL, depth);
}

void Main::getTermUnion(List<TermInFile> s1, List<TermInFile> s2, List<TermInFile> *foundInDocs)
{
	s1.sort(); // sort both lists for a faster search
	s2.sort();

	ListNode<TermInFile> *s1Iterator = s1.startPtr; //create iterators for the lists
	ListNode<TermInFile> *s2Iterator = s2.startPtr;

	while (s1Iterator != NULL && s2Iterator != NULL)// start iterating
	{
		if (s1Iterator->data.id < s2Iterator->data.id)// if s1.id < s2.id move s1 iterator forward and add data
		{
			foundInDocs->insert(s1Iterator->data);
			s1Iterator = s1Iterator->nextPtr;
		}
		else if (s1Iterator->data.id > s2Iterator->data.id)// if s1.id > s2.id move s2 iterator forward and add data
		{
			foundInDocs->insert(s2Iterator->data);
			s2Iterator = s2Iterator->nextPtr;
		}
		else // if s1.id == s2.id
		{
			foundInDocs->insert(s1Iterator->data); // add data
			s1Iterator = s1Iterator->nextPtr;
		}
	}

	while (s1Iterator != NULL) //check if there is some data left in the lists and insert it into the new list
	{
		foundInDocs->insert(s1Iterator->data);
		s1Iterator = s1Iterator->nextPtr;
	}

	while (s2Iterator != NULL)
	{
		foundInDocs->insert(s2Iterator->data);
		s2Iterator = s2Iterator->nextPtr;
	}
}

/*
	finds the terms document intersection
*/
void Main::getTermIntersection(List<TermInFile> s1, List<TermInFile> s2, List<TermInFile> *foundInDocs)
{
	s1.sort(); // sort both lists for a faster search
	s2.sort();

	ListNode<TermInFile> *s1Iterator = s1.startPtr; //create iterators for the lists
	ListNode<TermInFile> *s2Iterator = s2.startPtr;

	while (s1Iterator != NULL && s2Iterator != NULL)// start iterating
	{
		if (s1Iterator->data.id < s2Iterator->data.id)// if s1.id < s2.id move s1 iterator forward
		{
			s1Iterator = s1Iterator->nextPtr;
		}
		else if (s1Iterator->data.id > s2Iterator->data.id)// if s1.id > s2.id move s2 iterator forward
		{
			s2Iterator = s2Iterator->nextPtr;
		}
		else// if s1.id == s2.id
		{
			foundInDocs->insert(s2Iterator->data); //insert the data
			s1Iterator = s1Iterator->nextPtr;
		}
	}
}

/*
	finds the documents where the both terms are side by side in that order
*/
void Main::getTermWith(List<TermInFile> s1, List<TermInFile> s2, List<TermInFile> *foundInDocs)
{
	s1.sort(); // sort both lists for a faster search
	s2.sort();

	ListNode<TermInFile> *s1Iterator = s1.startPtr; //create iterators for the lists
	ListNode<TermInFile> *s2Iterator = s2.startPtr;

	while (s1Iterator != NULL && s2Iterator != NULL) // start iterating
	{
		if (s1Iterator->data.id < s2Iterator->data.id) // if s1.id < s2.id move s1 iterator forward
		{
			s1Iterator = s1Iterator->nextPtr;
		}
		else if (s1Iterator->data.id > s2Iterator->data.id) // if s1.id > s2.id move s2 iterator forward
		{
			s2Iterator = s2Iterator->nextPtr;
		}
		else // if s1.id == s2.id
		{
			ListNode<int> *tempiterFirst = s1Iterator->data.positions.startPtr; //create iterator for first word positions

			while (tempiterFirst != NULL)
			{
				ListNode<int> *tempiterSecond = s2Iterator->data.positions.startPtr; //create iterator for second word positions
				while (tempiterSecond != NULL)
				{
					if (tempiterSecond->data - tempiterFirst->data == 1) // if the second word position - the first word position is equal to 1 insert the data
					{
						foundInDocs->insert(s1Iterator->data);
					}
					tempiterSecond = tempiterSecond->nextPtr;
				}
				tempiterFirst = tempiterFirst->nextPtr;
			}

			s1Iterator = s1Iterator->nextPtr;
		}
	}
}


