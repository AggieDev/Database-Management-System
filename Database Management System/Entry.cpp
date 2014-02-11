<<<<<<< HEAD
#include "stdafx.h"
=======

>>>>>>> origin/master
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
	cout << "\n";
}
<<<<<<< HEAD
=======

ostream& operator<<(ostream& out, const Entry& e)
{
	for (int i = 0; i < e.getFields().size(); i++)
	{
		out << e.getFields().at(i) + "\t";
	}
	return out;
}

>>>>>>> origin/master
