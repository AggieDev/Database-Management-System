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

	
	// return a new table where every included Entry satisfies the condition vector
	static Table select(vector<string> condition, Table table);
	

    //atributes = {"id","name","age"} or {"*"}
	static Table Project(vector<string> attributes, Table fromTable);
    
    
	static Table productTable(Table t1, Table t2);
	static Table naturalJoinTable(Table t1, Table t2);

	//accessors
	static vector<Table> getTables(){ return _tables; }
	static Table getTable(string relationName);
	static Table* getTableByReference(string relationName);
	static Table rename_table(Table* fromTable, vector<string> new_attributes);

	//returns -1 if not found, index of table if found
	static int tableExists(string);
};

