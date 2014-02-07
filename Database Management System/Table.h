// class to represent a table/relation in the database
#pragma once

#include <iostream>
#include <vector>
#include <string>

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

// CONSTRUCTOR
	Table(string name, int numCols=2);// create table

// ACCESSORS
	
	Entry getEntry(int entryID);
	Entry getEntry(string name, int col=0);
	
	string getName() { return _name; }
	int getNumCols() { return _numCols; }

// MUTATORS
			// the following return true on success
	bool addEntry(Entry e);				// insert into
	bool addEntry(vector<string> fields);

	void dropTable();
	bool update(string key, string newVal, int col=0);
	bool deleteEntry(int row);
	bool deleteEntry(string key, int col=0);


	
};
struct Entry{
	vector<string> fields;
};

struct Unicorn{
	vector<string> babyUnicorns;
};
