#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "Database.h"
#include "Table.h"

using namespace std;

int main(int argc, const char* argv[])
{
	cout << "DBMS started.\n";

	Database database = Database();

	vector<string> columnNames;
	columnNames.push_back("Names");
	columnNames.push_back("Ages");
	Table first = Table("DiffTest1", columnNames, 2);
	//Table second = Table("DiffTest2", 2);
	vector<string> firstEntVec;

	firstEntVec.push_back("Bob");
	firstEntVec.push_back("1");
	

	firstEntVec.push_back("first col");
	firstEntVec.push_back("second col");
	firstEntVec.push_back("third col");

	first.addEntry(Entry(firstEntVec));
	first.printTable();

	//to be added later -WB
	//database.addTable(first);
	//database.addTable(second);


	//difference (will uncomment once Table has fixed compile errors)
	//Table diff = database.differences(first, second);

	cout << "\n\n";
	system("PAUSE");
}