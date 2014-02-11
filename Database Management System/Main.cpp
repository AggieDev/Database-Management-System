#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "Database.h"
#include "Table.h"

using namespace std;

//usint this to test occupying tables
void testOccupyTables(Table &first, Table &second)
{
	vector<string> entryVec;

	entryVec.push_back("Bob");
	entryVec.push_back("1");
	first.addEntry(Entry(entryVec));

	entryVec.clear();

	entryVec.push_back("Anne");
	entryVec.push_back("3");
	first.addEntry(Entry(entryVec));

	entryVec.clear();

	entryVec.push_back("Dog");
	entryVec.push_back("2");
	first.addEntry(Entry(entryVec));

	entryVec.clear();
	first.printTable();

	entryVec.push_back("Bob");
	entryVec.push_back("3");
	second.addEntry(Entry(entryVec));

	entryVec.clear();

	entryVec.push_back("Anne");
	entryVec.push_back("3");
	second.addEntry(Entry(entryVec));

	entryVec.clear();
	second.printTable();
}

//using this to test difference table
void testDifferenceTable(Database &database)
{
	//create and occupy 2 tables with data
	vector<string> columnNames;
	columnNames.push_back("Names");
	columnNames.push_back("Ages");
	Table first = Table("DiffTest1", columnNames, 2);
	Table second = Table("DiffTest2", columnNames, 2);

	testOccupyTables(first, second);

	//test difference relation
	database.addTable(first);
	database.addTable(second);

	Table diff = database.differenceTable(first, second);
	diff.printTable();
}


int main(int argc, const char* argv[])
{
	cout << "DBMS started.\n";

	Database database = Database();

	testDifferenceTable(database);

	cout << "\n\n";
	system("PAUSE");
}