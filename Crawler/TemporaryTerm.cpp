#include "TemporaryTerm.h"


TemporaryTerm::TemporaryTerm() {}

bool TemporaryTerm::operator==(TemporaryTerm data)
{
	return term == data.term;
}