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

	vector<Entry> _entries;	// vector of vectors is an easier implementation...
	//string* _arrEntries[2];		// array of arrays uses contiguous memory, good when searching columns
	//char*** _ptrEntries;		// maintains a pointer to a block of memory containing-
								//		pointers that point to blocks of memory containing-
								//			pointers that point to blocks of memory containing-
								//				chars

public:

	Table(string name, int numCols=2);
	~Table();

// ACCESSORS
	
	Entry getEntry(unsigned int entryID);
	//Entry getEntry(string name, int col=0);
	
	string getName() { return _name; }
	int getNumCols() { return _numCols; }

// MUTATORS

	void addEntry(Entry e);				// insert into
	void addEntry(vector<string> fields);

	void dropTable();
	bool update(string key, string newVal, int keyCol=0, int valCol=1);
	
	bool deleteEntryRow(unsigned int row);
	bool deleteEntry(string key, int keyCol=0);

	bool importDB(string fileName); 
	
};
