#include <iostream>

/*
	this class is used for the already indexed pages;
*/

class IndexedPage
{
public:
	int id; // id of the page
	std::string pageName; // name of the page
	bool operator>(IndexedPage); // predefined operator used for sorting
};