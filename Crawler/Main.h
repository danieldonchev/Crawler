

#include <iostream>
#include "Tree.h"
#include "HttpClient.h"
#include "IndexedPage.h"


/*
	this class is used to store global variables and functions/utilities
*/
class Main
{
public:
	static int id;
	static Tree tree;
	static List<IndexedPage> indexedPages; // all indexed pages in the crawler
	static List<Http> Pages; // all indexed pages with their text and links etc..
	static void startIndexing(std::string url, int depth); //start the indexing of the site
	static void getTermIntersection(List<TermInFile> s1, List<TermInFile> s2, List<TermInFile> *foundInDocs); // find the intersection of documents in two lists
	static void getTermUnion(List<TermInFile> s1, List<TermInFile> s2, List<TermInFile> *foundInDocs); // find the union of documents in two lists
	static void getTermWith(List<TermInFile> s1, List<TermInFile> s2, List<TermInFile> *foundInDocs); // check if words are next to each other in order
};