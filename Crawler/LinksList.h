#include <iostream>
#include <string>
#include "LinksListItem.h"

class LinksList
{
public:
	std::string text;
	LinksListItem *head;

	LinksList();
	void addToList(std::string text);
};