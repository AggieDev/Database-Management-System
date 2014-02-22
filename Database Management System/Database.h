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

	
	// This select method should return a new table that is a subset of the provided table,
	// where every full Entry is included that satisfies the condition vector

	// for now, expect calls from Parser::evaluateCondtion to pass a vector where condition.size()==3
	//	and the contents are [operand1, operator, operand2]

	//  operand ::= attribute-name | literal
	//			where attribute-name is an identifier
	//			  and literal is a word(s) surround in quotes
	//		a literal in our DML would look like "red" or "Dinosaur"
	//		but when it gets parsed, an equivalent c++ string looks like "\"red\"" or "\"Dinosaur\""
	static Table select(vector<string> condition, Table table);
	



	static Table Project(vector<string> attributes, string fromTable);
    //atributes = {"id","name","age"} or {"*"}
	static Table Project(vector<string> attributes, Table* fromTable);
    
    
	static Table productTable(Table t1, Table t2);
	static Table naturalJoinTable(Table t1, Table t2);

	//accessors
	static vector<Table> getTables(){ return _tables; }
	static Table getTable(string relationName);
	static Table* getTableByReference(string relationName);
	static Table rename_table(Table* fromTable, vector<string> new_attributes);
};

