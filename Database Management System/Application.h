#pragma once
#include "Database.h"
#include "Table.h"
#include "Entry.h"

class Application
{
	//keep application functions private, that way all events can happen through
	//the application class itself and not main 
	//each of these returns a query to be executed
	string createTable(string tableName, vector<string>, vector<char>);
	string insertIntoTable(string tableName, Entry& e);
	string showTable(string tableName);	//will change how this works if using GUI
	string unionTable(string, string, string);
	string differenceTable(string, string, string);
	string productTable(string, string, string);
	string updateTable(string, string, string, int, int);
	string deleteFromTable(string tableName);
	string tableSelect(string tableName);
	string renameTable(string, string, vector<string>);
	string writeTable(string tableName);	//does this mean write to file?
	string tableProjection();

public:
	Application();
	~Application();

	void launch();
};

