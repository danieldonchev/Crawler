#include "TermInFile.h"


bool TermInFile::operator>(TermInFile data)
{
	return this->id > data.id;
}

bool TermInFile::operator==(TermInFile data)
{
	return this->fileName == data.fileName;
}