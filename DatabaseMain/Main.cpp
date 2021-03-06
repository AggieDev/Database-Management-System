
#include <stdio.h>
#include <iostream>
#include "Database.h"
#include "Table.h"
#include "Parser.h"
#include "Application.h"

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
void testDifferenceTable()
{
	cout << "\n=======Testing Difference Table=======\n\n";
	//create and occupy 2 tables with data
	vector<string> columnNames;
	columnNames.push_back("Names");
	columnNames.push_back("Ages");
	Table first = Table("DiffTest1", columnNames, 2);
	Table second = Table("DiffTest2", columnNames, 2);

	testOccupyTables(first, second);

	//test difference relation
	Database::addTable(first);
	Database::addTable(second);

	Table diff = Database::differenceTable(first, second);
	diff.printTable();
}

void testProductTable()
{
	cout << "\n=======Testing Product Table=======\n\n";
	Database::printTables();
	Table product = Database::productTable(Database::getTables().at(0), Database::getTables().at(1));
	product.printTable();

}

void testSelection()
try {
	cout << "\n=======Testing Select Table=======\n\n";
	vector<string> attributes;
	vector<char> attTypes;
	attributes.push_back("FirstName");
	attTypes.push_back('s');
	attributes.push_back("LastName");
	attTypes.push_back('s');
	attributes.push_back("Age");
	attTypes.push_back('i');
	attributes.push_back("Price");
	attTypes.push_back('f');

	Table table("Testing", attributes, attTypes);

	vector<string> entry1;
	entry1.push_back("Eliutt");
	entry1.push_back("Rivera");
	entry1.push_back("20");
	entry1.push_back("20.50");
	vector<string> entry2;
	entry2.push_back("Bob");
	entry2.push_back("Sagget");
	entry2.push_back("40");
	entry2.push_back("50.25");
	table.addEntry(entry1);
	table.addEntry(entry2);

	vector<string> selectAttr;
	selectAttr.push_back("*");
	//selectAttr.push_back("LastName");
	//selectAttr.push_back("Age");
	//selectAttr.push_back("Price");

	vector<string> selectWhere;
	selectWhere.push_back("Price"); //left
	selectWhere.push_back("!="); //center
	selectWhere.push_back("20.5"); //right
	Database::addTable(table);

	table.printTable();

//	Table results = Database::select(selectAttr, "Testing", selectWhere);

	//results.printTable();

}
catch (string error)
{
	throw error;
}

void testProjection()
try {
	cout << "\n=======Testing Project Table=======\n\n";

	vector<string> attributes;
	vector<char> attTypes;
	attributes.push_back("FirstName");
	attTypes.push_back('s');
	attributes.push_back("LastName");
	attTypes.push_back('s');
	attributes.push_back("Age");
	attTypes.push_back('i');
	attributes.push_back("Price");
	attTypes.push_back('f');
	Table table("Test", attributes, attTypes);

	vector<string> entry1;
	entry1.push_back("Eli");
	entry1.push_back("Riv");
	entry1.push_back("1");
	entry1.push_back("45");

	vector<string> entry2;
	entry2.push_back("pat");
	entry2.push_back("green");
	entry2.push_back("32");
	entry2.push_back("50.25");

	table.addEntry(entry1);
	table.addEntry(entry2);

	vector<string> projectAttr;
	projectAttr.push_back("*");
	//projectAttr.push_back("LastName");


	Database::addTable(table);

	table.printTable();

	//Table results = Database::Project(projectAttr, "Test");

//	results.printTable();

}
catch (string error)
{
	throw error;
}


void testSetUnion()
{
	//create and occupy 2 tables with data
	cout << "\n=======Testing Union Table=======\n\n";
	vector<string> columnNames;
	columnNames.push_back("Names");
	columnNames.push_back("Ages");
	Table first = Table("UnionTest1", columnNames, 2);
	Table second = Table("UnionTest2", columnNames, 2);

	testOccupyTables(first, second);

	//test union relation
	Database::addTable(first);
	Database::addTable(second);

	Table union_table = Database::setunion(first, second);
	union_table.printTable();
}

int main(int argc, const char* argv[])
{
	cout << "DBMS started.\n";

	/*
	testDifferenceTable();
	testProductTable();
	try
	{

		testSelection();
	}
	catch (string error)
	{
		cout << error << endl;
	}
	try
	{

		testProjection();
	}
	catch (string error)
	{
		cout << error << endl;
	}
	testSetUnion();*/

	Application application = Application();
	application.launch();
}