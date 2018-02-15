#include "IndexedPage.h"

bool IndexedPage::operator>(IndexedPage page)
{
	return this->id > page.id;
}