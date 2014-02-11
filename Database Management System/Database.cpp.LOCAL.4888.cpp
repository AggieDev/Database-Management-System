#include "stdafx.h"
#include "Database.h"


Database::Database()
{
}


Database::~Database()
{
}

Table Database::differenceTable(Table t1, Table t2)
{
	vector<string> columnNames;
	columnNames.push_back("Names");
	columnNames.push_back("Ages");
	Table diffTable = Table(t1.getName() + " difference " + t2.getName(), columnNames, 2);
	diffTable = t1;														//copy first table
	diffTable.setName(t1.getName() + " difference " + t2.getName());	//set name back 

	//delete entries that are in t1 and t2
	for (int i = 0; i < t2.getEntries().size(); i++)
	{
		//if diffTable contains this t2 entry, delete it
		if (diffTable.hasEntry(t2.getEntries().at(i)) != 0)
		{
			diffTable.deleteEntryRow(diffTable.hasEntry(t2.getEntries().at(i)));
		}
	}

	return diffTable;
}

Table Database::productTable(Table t1, Table t2)
{
	//4 columns due to multiplication
	vector<string> colNames;
	colNames.push_back("Name");
	colNames.push_back("Age");
	colNames.push_back("Name");
	colNames.push_back("Age");
	Table productTable = Table(t1.getName() + " cross " + t2.getName(), colNames, 4);
	//for each entry in t1
	for (int i = 0; i < t1.getEntries().size(); i++)
	{
		//for each entry in t2
		for (int j = 0; j < t2.getEntries().size(); j++)
		{
			vector<string> entryVec;
			entryVec.push_back(t1.getEntries().at(i).getFields().at(0));
			entryVec.push_back(t1.getEntries().at(i).getFields().at(1));
			entryVec.push_back(t2.getEntries().at(j).getFields().at(0));
			entryVec.push_back(t2.getEntries().at(j).getFields().at(1));
			productTable.addEntry(Entry(entryVec));
		}
	}

	return productTable;
}

void Database::printTables()
{
	for (int i = 0; i < _tables.size(); i++)
		_tables.at(i).printTable();
}