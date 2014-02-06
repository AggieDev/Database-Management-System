// class to represent a table/relation in the database

#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <vector>

using namespace std;


class Table{
private:
	string _name;
	int _numFields;
	vector<Entry> _entries;
public:

	// constructor
	Table();
	Table(string name);
	Table(string name, int numFields);

	// accessors
	

	// mutators
	delete

	
};
struct Entry{
	vector<string> _fields;
};

#endif