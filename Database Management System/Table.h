// class to represent a table/relation in the database
#pragma once

#include <iostream>
#include <vector>
#include <string>

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
	vector<Entry> _entries;	// vector of vectors is an easier implementation...
	//string* _arrEntries[2];		// array of arrays uses contiguous memory, good when searching columns
	//char*** _ptrEntries;		// maintains a pointer to a block of memory containing-
	//		pointers that point to blocks of memory containing-
	//			pointers that point to blocks of memory containing-
	//				chars

public:
	Table() {}
	Table(string name, int numCols = 2);
	Table(string name, vector<string> cols, int numCols = 2){ _name = name; colNames = cols; }
	Table(string name, vector<string> cols, vector<char> types){ _name = name; colNames = cols; colTypes = types; _numCols = cols.size();}
	~Table();
	void printTable();

	// ACCESSORS
	vector<string> getColNames() const { return colNames; }
	vector<char> getColTypes() const {return colTypes; }
	Entry getEntry(unsigned int entryID);
	//Entry getEntry(string name, int col=0);
	vector<Entry> getEntries(){ return _entries; }

	string getName() { return _name; }
	int getNumCols() { return _numCols; }

	// MUTATORS

	int hasEntry(Entry e);	//returns index of matched entry, unless it doesn't exist then it returns 0
    

	void addColumn(string colName, char colType); //USE THIS FUNCTION TO ADD MORE COLUMNS

	void addEntry(Entry e);				// insert into
	void addEntry(vector<string> fields);
	void setName(string name){ _name = name; }

	void dropTable();
	bool update(string key, string newVal, int keyCol = 0, int valCol = 1);

	vector<int> findCondition(vector<string> whereOps); //returns index of entries that fulfill the condition

	bool deleteEntryRow(unsigned int row);
	bool deleteEntry(string key, int keyCol = 0);

	bool importDB(string fileName);
	void rename(vector<string> new_attributes);

};
