#include "LinksList.h"

LinksList::LinksList()
{}

void LinksList::addToList(std::string text)
{
	LinksListItem *node = new LinksListItem(text, NULL);
	LinksListItem *current = new LinksListItem();

	if (head == NULL)
	{
		head = node;
		current = node;
	}
	else
	{
		current = head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = node;
	}
}

