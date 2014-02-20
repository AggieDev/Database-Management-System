#pragma once

#include "Table.h"

class Database
{
private:
	static vector<Table> _tables;
	Database();
	~Database();

public:
	//database is global (static)

	static void printTables();
	static void addTable(Table t){ _tables.push_back(t); }
	static Table differenceTable(Table t1, Table t2);
	static Table setunion(Table t1, Table t2);
	//relational functions
	static Table select(vector<string> attributes, string fromTable, vector<string> _where);
	//atributes = {"id","name","age"} or {"*"}
	//_where = {"age","<","10"} parse the where operations

	static Table select(vector<string> attributes, Table* fromTable, vector<string> _where);
   
	static Table Project(vector<string> attributes, string fromTable);
    //atributes = {"id","name","age"} or {"*"}
	static Table Project(vector<string> attributes, Table* fromTable);
    
    
	static Table productTable(Table t1, Table t2);
	static Table naturalJoinTable(Table t1, Table t2);

	//accessors
	static vector<Table> getTables(){ return _tables; }
	static Table getTable(string relationName);
	static Table* getTableByReference(string relationName);
};

