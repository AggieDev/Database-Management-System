#pragma once

#include "Table.h"

class Database
{
private:
	vector<Table> _tables;
public:
	Database();
	~Database();

	void printTables();
	void addTable(Table t){ _tables.push_back(t); }

	//relational functions
	Table select(vector<string> attributes, string fromTable, vector<string> _where);
	//atributes = {"id","name","age"} or {"*"}
	//_where = {"age","<","10"} parse the where operations

   
		//atributes = {"id","name","age"} or {"*"}
		//_where = {"age","<","10"} parse the where operations
    Table Project(vector<string> attributes, string fromTable);
	Table differenceTable(Table t1, Table t2);
	Table productTable(Table t1, Table t2);
	Table naturalJoinTable(Table t1, Table t2);

	//accessors
	vector<Table> getTables(){ return _tables; }
	Table setunion(Table t1, Table t2);
};

