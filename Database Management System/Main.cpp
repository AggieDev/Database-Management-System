
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


void testSelection()
try {
	
	Database database;
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

	Table table("Testing",attributes,attTypes);
	vector<string> entry1;
	entry1.push_back("Eliutt");
	entry1.push_back("Rivera");
	entry1.push_back("10");
	entry1.push_back("20.50");
	vector<string> entry2;
	entry2.push_back("Bob");
	entry2.push_back("Sagget");
	entry2.push_back("40");
	entry2.push_back("50.25");
	table.addEntry(entry1);
	table.addEntry(entry2);

	vector<string> selectAttr;
	selectAttr.push_back("FirstName");
	
	vector<string> selectWhere;
	selectWhere.push_back("Age"); //left
	selectWhere.push_back(">"); //center
	selectWhere.push_back("20"); //right
	database.addTable(table);

	table.printTable();

	Table results = database.select(selectAttr,"Testing",selectWhere);

	results.printTable();
	
}catch(string error)
{
	throw error;
}

void testProjection()
try {
	
	Database database;
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
    Table table("Testing2",attributes,attTypes);
    
	vector<string> entry1;
	entry1.push_back("Eliutt");
	entry1.push_back("Rivera");
	entry1.push_back("10");
	entry1.push_back("20.50");
    
	vector<string> entry2;
	entry2.push_back("Bob");
	entry2.push_back("Sagget");
	entry2.push_back("40");
	entry2.push_back("50.25");
    
	table.addEntry(entry1);
	table.addEntry(entry2);
    
	vector<string> projectAttr;
	projectAttr.push_back("FirstName");
	
	database.addTable(table);
    
	table.printTable();
    
	Table results = database.Project(projectAttr,"Testing2");
    
    results.printTable();
	
}catch(string error)
{
	throw error;
}


int main(int argc, const char* argv[])
{
	cout << "DBMS started.\n";

	Database database = Database();

	testDifferenceTable(database);
	try
	{
		testSelection();
        testProjection();
	}
	catch(string error)
	{
		cout << error << endl;
	}
	cout << "\n\n";
	system("PAUSE");
}