// class to represent a table/relation in the database
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Entry.h"

using namespace std;

class Table
{
private:
	string _name;
	int _numCols;
	vector<char> colTypes; //should always be the same size as colNames. colNames[0] = "FirstName", colType[0] = 's'
	//'s' - string
	//'f' - float (double)
	//'i' - int
	vector<string> colNames;
	vector<Entry> _entries;	


public:
	Table() {}
	Table(string name, int numCols = 2);
	Table(string name, vector<string> cols, int numCols = 2){ _name = name; colNames = cols; _numCols = numCols; }
	Table(string name, vector<string> cols, vector<char> types){ _name = name; colNames = cols; colTypes = types; _numCols = cols.size(); }
	~Table();
	void printTable();

	// ACCESSORS
	vector<string> getColNames() const { return colNames; }
	vector<char> getColTypes() const { return colTypes; }
	Entry getEntry(unsigned int entryID);
	//Entry getEntry(string name, int col=0);
	vector<Entry> getEntries(){ return _entries; }
	void setColTypes(std::string col_types);
	string getName() { return _name; }
	int getNumCols() { return _numCols; }

	// MUTATORS

	int hasEntry(Entry e);	//returns index of matched entry, unless it doesn't exist then it returns -1
	void addColumn(string colName, char colType); //USE THIS FUNCTION TO ADD MORE COLUMNS
	void addEntry(Entry e);				// insert into
	void addEntry(vector<string> fields);
	void setName(string name){ _name = name; }
	void setNumCols(int num_cols);
	void dropTable();
	bool update(string key, string newVal, int keyCol = 0, int valCol = 1);

	vector<int> findCondition(vector<string> condition); //returns index of entries that fulfill the condition

	bool deleteEntryRow(unsigned int row);
	bool deleteEntry(string key, int keyCol = 0);

	bool importDB(string fileName);
	void rename(vector<string> new_attributes);
	void open_file(std::string relation_name);
	void close_file(string relationName);
	void write_to_file(string relationName);

	// Utilities
	bool stringOperatorCompare(string operand1, string op, string operand2);
};
