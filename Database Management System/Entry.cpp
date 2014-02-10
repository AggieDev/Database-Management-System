#include "stdafx.h"
#include "Entry.h"

Entry::Entry()
{
	// merge test
}
Entry::~Entry()
{
}

void Entry::printEntry()
{
	for (int i = 0; i < fields.size(); i++)
	{
		cout << fields.at(i) + "\t";
	}
}
