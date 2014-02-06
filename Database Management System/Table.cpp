#include "Table.h"


Table::Table(string name, int numFields=2)
{ // create a new empty table
	_name = name;
	_numFields = numFields;
	_vecEntries.clear();
}
