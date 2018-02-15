#include <iostream>
#include <string>

class LinksListItem
{
public:
	std::string text;
	LinksListItem *next;

	LinksListItem();
	LinksListItem(std::string _text);
	LinksListItem(std::string _text, LinksListItem *_next);
};