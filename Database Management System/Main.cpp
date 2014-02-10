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

	//MAKE THIS WORK
	//Table first = Table("DiffTest1", 2);
	//Table second = Table("DiffTest2", 2);
	//first.addEntry(new Entry
	//database.addTable(first);
	//database.addTable(second);


	//difference (will uncomment once Table has fixed compile errors)
	//Table diff = database.differences(first, second);


	system("PAUSE");
}