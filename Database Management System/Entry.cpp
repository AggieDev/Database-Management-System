
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
	for (unsigned int i = 0; i < fields.size(); i++)
	{
		cout << fields.at(i) + "\t";
	}
	cout << "\n";
}

ostream& operator<<(ostream& out, const Entry& e)
{
	for (unsigned int i = 0; i < e.getFields().size(); i++)
	{
		out << e.getFields().at(i) + "\t";
	}
	return out;
}

