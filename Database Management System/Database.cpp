<<<<<<< HEAD
#include "stdafx.h"
#include "Database.h"

=======

#include "Database.h"
#include <stdlib.h> //atoi, atof
>>>>>>> origin/master

Database::Database()
{
}

<<<<<<< HEAD

=======
>>>>>>> origin/master
Database::~Database()
{
}

Table Database::differenceTable(Table t1, Table t2)
{
	vector<string> columnNames;
	columnNames.push_back("Names");
	columnNames.push_back("Ages");
	Table diffTable = Table(t1.getName() + " difference " + t2.getName(), columnNames, 2);
	diffTable = t1;	//copy first table

	//delete entries that are in t1 and t2
	for (int i = 0; i < t2.getEntries().size(); i++)
	{
		//if diffTable contains this t2 entry, delete it
		if (diffTable.hasEntry(t2.getEntries().at(i)) != 0)
		{
			diffTable.deleteEntryRow(diffTable.hasEntry(t2.getEntries().at(i)));
		}
	}

	return diffTable;
}

<<<<<<< HEAD
Table Database::setunion(Table t1, Table t2)
{
	bool match = true;
	Table union_table("Union Table", t1.getNumCols());
	vector<Entry> entry_vec;
	for (int i = 0; i < t1.getEntries().size(); i++)
	{
		entry_vec.push_back(t1.getEntries().at(i));
	}

	for (int i = 0; i < entry_vec.size(); i++){
		union_table.addEntry(entry_vec.at(i));
	}

	for (int i = 0; i < t2.getEntries().size(); i++)
	{
		//match = false;
		match = true;
		//union_table.addEntry(entry_vec.at(i));
		for (int j = 0; j < t1.getEntries().size(); j++)
		{
			match = true;
			for (int k = 0; k < t1.getEntries().at(j).getFields().size(); k++)
			{
				if (t2.getEntries().at(i).getFields().at(k).compare(t1.getEntries().at(j).getFields().at(k)) == 0)
				{
					match = false;
					break;
				}
			}
			if (match == false && union_table.hasEntry(t2.getEntries().at(i)) == 0)
			{
				union_table.addEntry(t2.getEntries().at(i));
				std::cout << "something added \n";
				std::cout << i << "\n";
				break;
			}
		}
		

	}
	return union_table;
=======

Table Database::select(vector<string> attributes, string fromTable, vector<string> _where)
{
	Table* result;
	Table* selectedTable = NULL;
	for(int i = 0; i < _tables.size(); i++)
	{
		if (_tables[i].getName() == fromTable)
			selectedTable = &_tables[i];
	}
	if(selectedTable == NULL)
	{
		string error = "Error: Table " + fromTable + " does not exist.";
		throw error;
	}
	int count = 0;
	vector<char> colTypes;
	for(int i = 0; i < attributes.size(); i++)
	{
		for(int j = 0; j < selectedTable->getColNames().size(); j++)
		{
			if(attributes[i] == selectedTable->getColNames()[j])
			{
				colTypes.push_back(selectedTable->getColTypes()[j]);
				count++;
				break;
			}
		}
	}
	if(count < attributes.size()) //check if all attributes were found
	{
		string error = "Not all attributes were found in table " + selectedTable->getName();
		throw error;
	}
	vector<int> validEntries = selectedTable->findCondition(_where);


	if(attributes[0] == "*")
	{
		result = new Table("QueryResult",selectedTable->getColNames(),selectedTable->getColTypes());
		for(int i = 0; i < validEntries.size(); i++)
		{
			result->addEntry(selectedTable->getEntries()[i]);
		}
	}
	else
	{
		vector<int> columnsToSelect;
		for(int i =0; i < attributes.size(); i++)
		{
			for(int j = 0; j < selectedTable->getColNames().size(); j++)
			{
				if(attributes[i] == selectedTable->getColNames()[j])
					columnsToSelect.push_back(j);
			}
		}
		result = new Table("QueryResult",attributes,colTypes);
		for(int i = 0; i < validEntries.size(); i++) //error here
		{
			vector<string> fields;
			for(int j = 0; j < columnsToSelect.size(); j++)
			{
				fields.push_back(selectedTable->getEntries()[i][j]);
			}
			result->addEntry(fields);
		}
	}

	return *result;
>>>>>>> origin/master
}