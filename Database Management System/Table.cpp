#include "stdafx.h"
#include "Table.h"


Table::Table(string name, int numCols)
{ // create a new empty table
	_name = name;
	_numCols = numCols;
	_entries.clear();
}
Table::~Table()
{ // table destructor
	_entries.clear();  // NOTE: is this needed?
}

void Table::printTable()
{
	cout << "Table: " << _name << "\n" << "----------------\n"; 
	if (!colNames.empty())	//print column names
	{
		for (int i = 0; i < colNames.size(); i++)
			cout << colNames.at(i) << "\t";
		cout << "\n----------------\n";
	}
	//print entries
	for (int i = 0; i < _entries.size(); i++)
		_entries.at(i).printEntry();
}

Entry Table::getEntry(unsigned int entryID)
{ // return the entry at the provided row
	Entry entry;
	if (_entries.size() < entryID) 
	{
		throw new exception("entryID out of range in getEntry");
	}
	else 
	{
		entry = _entries.at(entryID);
	}
	return entry;
}
//Entry Table::getEntry(string name, int col=0){}

void Table::addEntry(Entry e)
{  // add new entry (row) to the database 
	_entries.push_back(e);
}
void Table::addEntry(vector<string> fields)
{ // add new entry (row) to database with the fields provided 
	if (fields.size() != _numCols)
	{
		throw new exception("in addEntry, incorrect number of fields provided for this table");
		return;
	}
	
	Entry e;
	for (unsigned int i = 0; i < fields.size(); i++)
	{
		e.fields.push_back(string(fields.at(i)));
	}
	_entries.push_back(e);
}

void Table::dropTable()
{ // clear entries vector
	_entries.clear();
}
bool Table::update(string key, string newVal, int keyCol, int valCol)
{ // update an existing entry with a new value, return true on success
	for (unsigned int i = 0; i < _entries.size(); i++)
	{
		Entry entry = _entries.at(i);
		string currKey = entry.fields.at(keyCol);
		if (!key.compare(currKey))
		{ // if the keys match
			entry.fields.at(valCol) = newVal;
			return true;
		}
	}
	return false;
}
bool Table::deleteEntryRow(unsigned int row)
{ // delete an existing entry, given the row number (zero-indexed). return true on success
	if (_entries.size() < row)
	{
		return false;
	}
	
	_entries.erase(_entries.begin() + row);
	return true;
}
bool Table::deleteEntry(string key, int keyCol)
{ // delete an existing entry whose value at the provided column is equal to the key
	for (vector<Entry>::iterator iter = _entries.begin(); iter != _entries.end(); iter++)
	{
		Entry entry = *iter;
		string currKey = entry.fields.at(keyCol);
		if (!key.compare(currKey))
		{
			_entries.erase(iter);
			return true;
		}
	}
	return false;
}

void Table::rename(vector<string> new_attributes){

}