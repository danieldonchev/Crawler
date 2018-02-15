#include "LinksList.h"

LinksListItem::LinksListItem()
{}

LinksListItem::LinksListItem(std::string _text)
{
	text = _text;
}

LinksListItem::LinksListItem(std::string _text, LinksListItem *_next)
{
	text = _text;
	next = _next;
}