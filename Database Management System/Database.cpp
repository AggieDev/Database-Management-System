//test
#include "Database.h"
#include <stdlib.h> //atoi, atof

Database::Database()
{
}

Database::~Database()
{
}

Table Database::naturalJoinTable(Table t1, Table t2)
{	// return naturally joint table of t1 and t2
	Table jointTable = Table(t1.getName() + " & " + t2.getName());
	
	vector<string> t1cols = t1.getColNames();
	vector<string> t2cols = t2.getColNames();

	vector<string> jointCols = t1cols;

	int count = 0;

	for (unsigned int j = 0; j < t1.getEntries().size(); j++)
	{
		jointTable.addEntry(t1.getEntries().at(j));
	}

	for (int i = 0; i < t2cols.size(); i++)
	{
		string curr = t2cols.at(i);
		if (find(jointCols.begin(), jointCols.end(), curr) != jointCols.end())
		{	// count how many similar columns there are
			count++;
		}
		else if (count > 0)
		{	// if the tables have one in common, and a new column is found
			jointCols.push_back(curr);
			for (unsigned int j = 0; j < t2.getEntries().size(); j++)
			{
				Entry currT2Entry = t2.getEntry(j);
				string key = currT2Entry.fields.at(0);
				string newVal = currT2Entry.fields.at(i);
				jointTable.update(key, newVal);
			}
		}
	}

	if (count > 0)
	{
		return jointTable;
	}
	else
	{
		return Table();
	}
}

Table Database::differenceTable(Table t1, Table t2)
{
	vector<string> columnNames;
	columnNames.push_back("Names");
	columnNames.push_back("Ages");
	Table diffTable = Table(t1.getName() + " difference " + t2.getName(), columnNames, 2);
	diffTable = t1;														//copy first table
	diffTable.setName(t1.getName() + " difference " + t2.getName());	//set name back 

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


Table Database::productTable(Table t1, Table t2)
{
	//4 columns due to multiplication
	vector<string> colNames;
	colNames.push_back("Name");
	colNames.push_back("Age");
	colNames.push_back("Name");
	colNames.push_back("Age");
	Table productTable = Table(t1.getName() + " cross " + t2.getName(), colNames, 4);
	//for each entry in t1
	for (int i = 0; i < t1.getEntries().size(); i++)
	{
		//for each entry in t2
		for (int j = 0; j < t2.getEntries().size(); j++)
		{
			vector<string> entryVec;
			entryVec.push_back(t1.getEntries().at(i).getFields().at(0));
			entryVec.push_back(t1.getEntries().at(i).getFields().at(1));
			entryVec.push_back(t2.getEntries().at(j).getFields().at(0));
			entryVec.push_back(t2.getEntries().at(j).getFields().at(1));
			productTable.addEntry(Entry(entryVec));
		}
	}

	return productTable;
}

void Database::printTables()
{
	for (int i = 0; i < _tables.size(); i++)
		_tables.at(i).printTable();
}

Table Database::select(vector<string> attributes, string fromTable, vector<string> _where)
{
	Table* result;
	Table* selectedTable = NULL;
	for (int i = 0; i < _tables.size(); i++)
	{
		if (_tables[i].getName() == fromTable)
			selectedTable = &_tables[i];
	}
	if (selectedTable == NULL)
	{
		string error = "Error: Table " + fromTable + " does not exist.";
		throw error;
	}
	int count = 0;
	vector<char> colTypes;
	for (int i = 0; i < attributes.size(); i++)
	{
		for (int j = 0; j < selectedTable->getColNames().size(); j++)
		{
			if (attributes[i] == selectedTable->getColNames()[j])
			{
				colTypes.push_back(selectedTable->getColTypes()[j]);
				count++;
				break;
			}
		}
	}
	if (count < attributes.size()) //check if all attributes were found
	{
		string error = "Not all attributes were found in table " + selectedTable->getName();
		throw error;
	}
	vector<int> validEntries = selectedTable->findCondition(_where);


	if (attributes[0] == "*")
	{
		result = new Table("QueryResult", selectedTable->getColNames(), selectedTable->getColTypes());
		for (int i = 0; i < validEntries.size(); i++)
		{
			result->addEntry(selectedTable->getEntries()[i]);
		}
	}
	else
	{
		vector<int> columnsToSelect;
		for (int i = 0; i < attributes.size(); i++)
		{
			for (int j = 0; j < selectedTable->getColNames().size(); j++)
			{
				if (attributes[i] == selectedTable->getColNames()[j])
					columnsToSelect.push_back(j);
			}
		}
<<<<<<< HEAD
		result = new Table("QueryResult", attributes, colTypes);
		for (int i = 0; i < validEntries.size(); i++) //error here
=======
		result = new Table("Select",attributes,colTypes);
		for(int i = 0; i < validEntries.size(); i++) //error here
>>>>>>> eliutt-branch
		{
			vector<string> fields;
			for (int j = 0; j < columnsToSelect.size(); j++)
			{
				fields.push_back(selectedTable->getEntries()[i][j]);
			}
			result->addEntry(fields);
		}
	}

	return *result;
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
				break;
			}
		}
		

	}
	return union_table;
}

=======
>>>>>>> eliutt-branch
Table Database::Project(vector<string> attributes, string fromTable)
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
    
    
	if(attributes[0] == "*")
	{
		result = new Table("Query",selectedTable->getColNames(),selectedTable->getColTypes());
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
<<<<<<< HEAD
		result = new Table("QueryResult",attributes);
		for (int i = 0; i < attributes.size(); i++) //error here
		{
			vector<string> fields;
			for (int j = 0; j < columnsToSelect.size(); j++)
			{
				fields.push_back(projectedTable->getEntries()[i][j]);
			}
			result->addEntry(fields);
		}
	}
=======

		result = new Table("Project",attributes,colTypes);
        
        /*vector<string> fields;
		for(int i = 0; i < columnsToSelect.size(); i++) //error here
		{
			fields.push_back(selectedTable->getEntries()[i]);
        }
        result->addEntry(fields);
		*/
    }
	
>>>>>>> eliutt-branch
    
	return *result;
}