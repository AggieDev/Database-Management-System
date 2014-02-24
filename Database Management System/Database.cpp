
#include "Database.h"
#include "Database.h"
#include <stdlib.h> //atoi, atof

vector<Table> Database::_tables;

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

	for (unsigned int i = 0; i < t2cols.size(); i++)
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
	for (unsigned int i = 0; i < t2.getEntries().size(); i++)
	{
		//if diffTable contains this t2 entry, delete it
		if (diffTable.hasEntry(t2.getEntries().at(i)) != 0)
		{
			diffTable.deleteEntryRow(diffTable.hasEntry(t2.getEntries().at(i)));
		}
	}

	return diffTable;
}

Table Database::setunion(Table t1, Table t2)
{
	vector<string> columnNames;
	columnNames.push_back("Names");
	columnNames.push_back("Ages");

	Table union_table = Table(t1.getName() + " Union " + t2.getName(), columnNames, 2);													//copy first table
	union_table.setName(t1.getName() + " Union " + t2.getName());	//set name back 

	for (unsigned int i = 0; i < t1.getEntries().size(); i++)
	{
		union_table.addEntry(t1.getEntries().at(i));
	}

	for (unsigned int i = 0; i < t2.getEntries().size(); i++){
		if (union_table.hasEntry(t2.getEntries().at(i)) == -1)
			union_table.addEntry(t2.getEntries().at(i));
	}

	return union_table;
	
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
	for (unsigned int i = 0; i < t1.getEntries().size(); i++)
	{
		//for each entry in t2
		for (unsigned int j = 0; j < t2.getEntries().size(); j++)
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
	for (unsigned int i = 0; i < _tables.size(); i++)
		_tables.at(i).printTable();
}


Table Database::select(vector<string> condition, Table fromTable)
{
	Table* result;
	Table* selectedTable = &fromTable;
	
	// eliminate parenthesis if present
	if (condition.at(0) == "(")

	{
		condition.erase(condition.begin());
	}
	if (condition.at(condition.size() - 1) == ")")
	{
		condition.erase(condition.begin() + condition.size() - 1);
	}

	if (condition.size() != 3)
	{	// check that condition vector is correct size
		throw new exception("Invalid condition vector in Database::select");
	}

	vector<int> validEntries = selectedTable->findCondition(condition);

	result = new Table("Result",selectedTable->getColNames(),selectedTable->getColTypes());
	for(unsigned int i = 0; i < validEntries.size(); i++)
	{
		result->addEntry(selectedTable->getEntries()[validEntries[i]]);
	}
	//else
	//{
	//	vector<int> columnsToSelect;
	//	for (unsigned int i = 0; i < attributes.size(); i++)
	//	{
	//		for (unsigned int j = 0; j < selectedTable->getColNames().size(); j++)
	//		{
	//			if(attributes[i] == selectedTable->getColNames()[j])
	//				columnsToSelect.push_back(j);
	//		}
	//	}
	//	result = new Table("Result",attributes,colTypes);
	//	for (unsigned int i = 0; i < validEntries.size(); i++) //error here
	//	{
	//		vector<string> fields;
	//		for (unsigned int j = 0; j < columnsToSelect.size(); j++)
	//		{
	//			fields.push_back(selectedTable->getEntries()[validEntries[i]][columnsToSelect[j]]);
	//		}
	//		result->addEntry(fields);
	//	}
	//}
 //   
	return *result;

}


Table Database::Project(vector<string> attributes, Table fromTable)
{
	Table* result;
	Table* selectedTable = NULL;
	selectedTable = &fromTable;
    
    
	unsigned int count = 0;
	vector<char> colTypes;
	for (unsigned int i = 0; i < attributes.size(); i++)
	{
		for (unsigned int j = 0; j < selectedTable->getColNames().size(); j++)
		{
			if(attributes[i] == selectedTable->getColNames()[j])
			{
				colTypes.push_back(selectedTable->getColTypes()[j]);
				count++;
				break;
			}
		}
	}
    
    /*if(attributes[0]=="*")count++;
    
	if(count < attributes.size()) //check if all attributes were found
	{
		string error = "Not all attributes were found in table " + selectedTable->getName();
		throw error;
	}
        
	if(attributes[0] == "*")
	{
		result = new Table("Result",selectedTable->getColNames(),selectedTable->getColTypes());
		for (unsigned int i = 0; i < selectedTable->getEntries().size(); i++)
		{
			result->addEntry(selectedTable->getEntries()[i]);
		}
	}*/
	
		vector<int> columnsToSelect;
		for (unsigned int i = 0; i < attributes.size(); i++)
		{
			for (unsigned int j = 0; j < selectedTable->getColNames().size(); j++)
			{
				if(attributes[i] == selectedTable->getColNames()[j])
					columnsToSelect.push_back(j);
			}
		}
        
		result = new Table("Project",attributes,colTypes);
        
		for (unsigned int i = 0; i < selectedTable->getEntries().size(); i++) 
		{
			vector<string> fields;
			for (unsigned int j = 0; j < columnsToSelect.size(); j++)
			{
				fields.push_back(selectedTable->getEntries()[i][columnsToSelect[j]]);
			}
			result->addEntry(fields);
		}
	
    
	return *result;

}


Table Database::rename_table(Table* fromTable, vector<string> new_attributes)
{
	Table* rename_table = fromTable;
	rename_table->rename(new_attributes);
	return *rename_table;
}



Table Database::getTable(string relationName)
{ // return pointer to the correct table, so it can be modified
	for (unsigned int i = 0; i < _tables.size(); i++)
	{
		if (_tables.at(i).getName() == relationName)
		{
			return _tables.at(i);
		}
	}
	return NULL;
}

Table* Database::getTableByReference(string relationName)
{ // return pointer to the correct table, so it can be modified
	for (unsigned int i = 0; i < _tables.size(); i++)
	{
		if (_tables.at(i).getName() == relationName)
		{
			return &_tables.at(i);
		}
	}
	return NULL;
}

int Database::tableExists(string tableName)
{
	//check each entry
	for (unsigned int i = 0; i < _tables.size(); i++)
	{
		if (tableName.compare(_tables.at(i).getName()))
			return i;
	}
	return -1;
}