#include <stdio.h>
#include <exception>
#include <stdexcept>
#include "Table.h"
#include "Database.h"
#include <iostream>

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
	cout << "\nTable: " << _name << "\n" << "----------------\n";
	if (!colNames.empty())	//print column names
	{
		for (int i = 0; i < colNames.size(); i++)
			cout << colNames.at(i) << "\t";
		cout << "\n----------------\n";
	}
	//print entries
	for (int i = 0; i < _entries.size(); i++)
		_entries.at(i).printEntry();
	cout << "\n";
}

int Table::hasEntry(Entry e)
{
	//check each entry
	for (int i = 0; i < _entries.size(); i++)
	{
		bool match = true;
		//see if all entry parameters match up
		for (int j = 0; j < _entries.at(i).getFields().size(); j++)
		{
			//if field at j doesnt match, entry isnt same so move on to next one
			if (e.getFields().at(j).compare(_entries.at(i).getFields().at(j)) != 0)
				match = false;
		}
		//if this last checked entry is a match, return that the entry exists
		if (match)
			return i;
	}
	return 0;
}

Entry Table::getEntry(unsigned int entryID)
{ // return the entry at the provided row
	Entry entry;
	if (_entries.size() < entryID)
	{
		throw string("entryID out of range in getEntry");
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
		throw string("in addEntry, incorrect number of fields provided for this table");
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


void Table::addColumn(string colName, char colType)
{
	colNames.push_back(colName);
	colTypes.push_back(colType);
}

void Table::setNumCols(int num)
{
	_numCols = num;
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
	if (new_attributes.size() == colNames.size()){
		colNames = new_attributes;

	}
	else std::cout << "ERROR: Please match the number of attributes.";

}

void Table::open_file(std::string table_name)
{
	std::string file_name = table_name;
	file_name += ".db";
	table_file.open(file_name);
	std::string num_cols;
	std::string col_types;
	std::string line;
	std::vector<string> line_vec;
	if (table_file.is_open())
	{
		while (getline(table_file, line))
		{
			line_vec.push_back(line);
		}
		std::cout << "File is open!\n";
	}
	setName(line_vec.at(0));
	num_cols = line_vec.at(1);
	setNumCols(std::stoi(num_cols));
	col_types = line_vec.at(2);
	colTypes = col_types.split(',');


}

void Table::setColTypes(std::string type_string)
{
	for (int i = 0; i < type_string.size(); i++)
	{
		colTypes.push_back(type_string.at(i));
	}
}

void Table::close_file()
{
	if (table_file.is_open())
	{
		table_file.close();
	}
	else std::cout << "File was not open!\n";
}

void Table::write_to_file()
{
	if (table_file.is_open())
	{
		table_file << _name << "\n";
		table_file << _numCols << "\n";
		for (int i = 0; i < colTypes.size(); i++)
		{
			table_file << colTypes.at(0) << ",";
		}
		table_file << "\n";
		for (int i = 0; i < colNames.size(); i++)
		{
			table_file << colNames.at(i) << ",";
		}
		table_file << "\n";
		for (int i = 0; i < _entries.size(); i++)
		{
			table_file << _entries.at(i) << ",";
		}
	}
	std::cout << "Writing to file complete!\n";
}

vector<int> Table::findCondition(vector<string> whereOps)
{
	vector<int> results;
	string left = whereOps[0];
	string center = whereOps[1];
	string right = whereOps[2];

	// whereOps = {"Age",  "<" , "10"}
	//            {left ,center,right}

	//find the column that corresponds to the 
	int columnToCheck = -1;
	for (int i = 0; i < colNames.size(); i++)
	{
		if (colNames[i] == left)
		{
			columnToCheck = i;
			break;
		}
	}
	if (columnToCheck == -1)
	{
		string error = "Error: " + left + " is not an Attribute in " + _name;
		throw error;
	}
	if (colNames.size() != colTypes.size())
	{
		string error = "We don't have the type for Attribute: " + left;
		throw error;
	}


	int intVal;
	double doubleVal;
	char type = colTypes[columnToCheck];
	bool isInt = false;
	switch (type)
	{
	case 'i':
		intVal = atoi(right.c_str());
		isInt = true;
	case 'f':
		doubleVal = atof(right.c_str());
		break;
	default: //probably string
		break;
	}

	for (int i = 0; i < _entries.size(); i++)
	{
		string valueToCheck = _entries[i][columnToCheck];
		if (center == "==")
		{
			if (isInt && atoi(valueToCheck.c_str()) == intVal)
				results.push_back(i);
			else if (!isInt && atof(valueToCheck.c_str()) == doubleVal)
				results.push_back(i);
		}
		else if (center == "<")
		{
			if (isInt && atoi(valueToCheck.c_str()) < intVal)
				results.push_back(i);
			else if (!isInt && atof(valueToCheck.c_str()) < doubleVal)
				results.push_back(i);
		}
        else if (center == "!=")
		{
			if (isInt && atoi(valueToCheck.c_str()) != intVal)
				results.push_back(i);
			else if (!isInt && atof(valueToCheck.c_str()) != doubleVal)
				results.push_back(i);
		}
		else if (center == ">")
		{
			if (isInt && atoi(valueToCheck.c_str()) > intVal)
				results.push_back(i);
			else if (!isInt && atof(valueToCheck.c_str()) > doubleVal)
				results.push_back(i);
		}
		else if (center == "<=")
		{
			if (isInt && atoi(valueToCheck.c_str()) <= intVal)
				results.push_back(i);
			else if (!isInt && atof(valueToCheck.c_str()) <= doubleVal)
				results.push_back(i);
		}
		else if (center == ">=")
		{
			if (isInt && atoi(valueToCheck.c_str()) >= intVal)
				results.push_back(i);
			else if (!isInt && atof(valueToCheck.c_str()) >= doubleVal)
				results.push_back(i);
		}
	}

	return results;

}

