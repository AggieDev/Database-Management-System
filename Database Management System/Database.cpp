
#include "Database.h"
#include <stdlib.h> //atoi, atof

Database::Database()
{
}

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
		result = new Table("Select",attributes,colTypes);
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
}

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

		result = new Table("Project",attributes,colTypes);
        
        /*vector<string> fields;
		for(int i = 0; i < columnsToSelect.size(); i++) //error here
		{
			fields.push_back(selectedTable->getEntries()[i]);
        }
        result->addEntry(fields);
		*/
    }
	
    
	return *result;
}