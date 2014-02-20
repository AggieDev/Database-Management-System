#pragma once
#include "Database.h"
#include "Table.h"
#include "Entry.h"

class Application
{
	//keep application functions private, that way all events can happen through
	//the application class itself and not main 
	void createTable(string tableName, vector<string> columnNames);
	Table insertIntoTable(string tableName, Entry& e);
	void showTable(string tableName);	//will change how this works if using GUI
	Table unionTable(string firstTable, string secondTable);
	Table differenceTable(string firstTable, string secondTable);
	Table productTable(string firstTable, string secondTable);
	void updateTable(string tableName);
	void deleteFromTable(string tableName);
	void tableSelect(string tableName);
	void renameTable(string tableName);
	void writeTable(string tableName);	//does this mean write to file?
	void tableProjection();

public:
	Application();
	~Application();

	void launch();
};

