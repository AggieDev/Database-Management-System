#pragma once
#include "Application.h"

using namespace std;

Application::Application()
{
}
Application::~Application()
{
}

void Application::launch()
{
	//application main code here
	//command line application structure:
	//give user a menu with an option for each of the functions in this class
	//   then ask user for inputs for each parameter of that function
	//each of these functions in this class returns a string that the dbms can call as a query

	string main = mainMenu();
	string tableName;

	if (main.compare("create") == 0)
		createPrompt();
	else if (main.compare("print") == 0)
		showPrompt();
	else if (main.compare("insert") == 0)
		insertPrompt();
	else if (main.compare("write") == 0)
		writePrompt();
	else if (main.compare("open") == 0)
		openPrompt();
	else if (main.compare("project") == 0)
		projectPrompt();
	else if (main.compare("rename") == 0)
		renamePrompt();
	else if (main.compare("union") == 0)
		unionPrompt();
	else if (main.compare("difference") == 0)
		differencePrompt();
	else if (main.compare("product") == 0)
		productPrompt();
	else if (main.compare("select") == 0)
		selectPrompt();
	else if (main.compare("update") == 0)
		updatePrompt();
	else if (main.compare("delete") == 0)
		deletePrompt();
		

		
	if (main.compare("exit") != 0)	//if not exiting, recall prompt
		launch();

	cout << "\n\n";
	system("PAUSE");
}

string Application::mainMenu()
{
	cout << "\n\nMain menu:\n--------\n\n";
	if (Database::getTables().size() == 0)
	{
		cout << "Because you have not created any tables, you must first create a table.\n\n";
		return "create";
	}
	else
	{
		while (true)	//break when correct command is given
		{
			cout << "Example - To create a table type \"create\" without quotes.\n\n";
			cout << "Create table: create\n";
			cout << "Insert into table: insert\n";
			cout << "Print a table: print\n";
			cout << "Write a table to file: write\n";		//is this correct?
			cout << "Open a table from file: open\n";
			cout << "Project table: project\n";
			cout << "Rename a table: rename\n";
			cout << "Union 2 tables: union\n";
			cout << "Difference table from 2 tables: difference\n";
			cout << "Product table from 2 tables: product\n";
			cout << "Select from table: select\n";
			cout << "Update a table: update\n";
			cout << "Delete from table: delete\n";
			cout << "Exit: exit\n\n";

			string input;
			getline(cin, input);
			if (input.compare("create") == 0 || input.compare("insert") == 0 || input.compare("print") == 0 || input.compare("write") == 0 || 
				input.compare("open") == 0 || input.compare("project") == 0 || input.compare("rename") == 0 || input.compare("union") == 0 || 
				input.compare("difference") == 0 || input.compare("product") == 0 || input.compare("select") == 0 || input.compare("update") == 0 || 
				input.compare("exit") == 0 || input.compare("delete") == 0)
			{
				return input;
			}
			else
				cout << input << " is not a valid command.\n\n";
			
		}
		
	}
}

void Application::printTableList()
{
	cout << "\nType a table name. Tables to choose from:\n";
	for (int i = 0; i < Database::getTables().size(); i++)
		cout << Database::getTables().at(i).getName() << "\n";
	cout << "\n";
}

void Application::createPrompt()
{
	string tableName;
	cout << "Table name: ";
	while (true)
	{
		getline(cin, tableName);
		if (tableName.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	string input;
	vector<string> columns;
	while (input.compare("done") != 0)
	{
		cout << "Enter a column name, if you are done entering columns names type 'done' without quotes:\n";
		getline(cin, input);

		if (input.compare("done") != 0)	//still runs once when done is entered
			columns.push_back(input);
	}
	cout << "\nTable " << tableName << " has been created.\n\n";

	vector<string> types;
	//for now, all are strings
	for (int i = 0; i < columns.size(); i++)
		types.push_back("VARCHAR(20)");

	cout << createTable(tableName, columns, types);
	Database::addTable(Table(tableName, columns));			//delete later, used for testing certain functions
}

void Application::insertPrompt()
{
	string tableName;
	while (true)
	{
		printTableList();
		getline(cin, tableName);
		if (tableName.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	if (!Database::tableExists(tableName))
		cout << "Table " << tableName << " doesn't exist.\n\n";
	else
	{
		vector<string> values;
		Table table = Database::getTable(tableName);
		for (int i = 0; i < table.getColNames().size(); i++)
		{
			cout << "Value for " << table.getColNames().at(i) << ": ";
			string input;
			getline(cin, input);

			values.push_back(input);
		}
		cout << insertIntoTable(tableName, values);
	}
}

void Application::showPrompt()
{
	string tableName;
	while (true)
	{
		printTableList();
		getline(cin, tableName);
		if (tableName.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	if (Database::tableExists(tableName))
		cout << showTable(tableName);
	else
		cout << "Table " << tableName << " doesn't exist.\n\n";
}

void Application::unionPrompt()
{
	string newTable;
	string firstTable;
	string secondTable;

	//new table name
	while (true)
	{
		cout << "Insert a name for the new table to be created: ";
		getline(cin, newTable);
		if (newTable.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	//first table name
	while (true)
	{
		cout << "\nType a table name for the first table to unionize. Tables to choose from:\n";
		for (int i = 0; i < Database::getTables().size(); i++)
			cout << Database::getTables().at(i).getName() << "\n";
		cout << "\n";
		getline(cin, firstTable);
		if (firstTable.size() == 0 || Database::tableExists(firstTable) == -1)	//empty name or table doesnt exits
			cout << "Please insert a valid table name.\n\n";
		else
			break;
	}

	//second table name
	while (true)
	{
		cout << "\nType a table name for the second table to unionize. Tables to choose from:\n";
		for (int i = 0; i < Database::getTables().size(); i++)
			cout << Database::getTables().at(i).getName() << "\n";
		cout << "\n";
		getline(cin, secondTable);
		if (secondTable.size() == 0 || Database::tableExists(secondTable) == -1)	//empty name or table doesnt exits
			cout << "Please insert a valid table name.\n\n";
		else
			break;
	}

	cout << unionTable(newTable, firstTable, secondTable);
}

void Application::differencePrompt()
{
	string newTable;
	string firstTable;
	string secondTable;

	//new table name
	while (true)
	{
		cout << "Insert a name for the new table to be created: ";
		getline(cin, newTable);
		if (newTable.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	//first table name
	while (true)
	{
		cout << "\nType a table name for the first table to unionize. Tables to choose from:\n";
		for (int i = 0; i < Database::getTables().size(); i++)
			cout << Database::getTables().at(i).getName() << "\n";
		cout << "\n";
		getline(cin, firstTable);
		if (firstTable.size() == 0 || Database::tableExists(firstTable) == -1)	//empty name or table doesnt exits
			cout << "Please insert a valid table name.\n\n";
		else
			break;
	}

	//second table name
	while (true)
	{
		cout << "\nType a table name for the second table to unionize. Tables to choose from:\n";
		for (int i = 0; i < Database::getTables().size(); i++)
			cout << Database::getTables().at(i).getName() << "\n";
		cout << "\n";
		getline(cin, secondTable);
		if (secondTable.size() == 0 || Database::tableExists(secondTable) == -1)	//empty name or table doesnt exits
			cout << "Please insert a valid table name.\n\n";
		else
			break;
	}

	cout << differenceTable(newTable, firstTable, secondTable);
}

void Application::productPrompt()
{
	string newTable;
	string firstTable;
	string secondTable;

	//new table name
	while (true)
	{
		cout << "Insert a name for the new table to be created: ";
		getline(cin, newTable);
		if (newTable.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	//first table name
	while (true)
	{
		cout << "\nType a table name for the first table to unionize. Tables to choose from:\n";
		for (int i = 0; i < Database::getTables().size(); i++)
			cout << Database::getTables().at(i).getName() << "\n";
		cout << "\n";
		getline(cin, firstTable);
		if (firstTable.size() == 0 || Database::tableExists(firstTable) == -1)	//empty name or table doesnt exits
			cout << "Please insert a valid table name.\n\n";
		else
			break;
	}

	//second table name
	while (true)
	{
		cout << "\nType a table name for the second table to unionize. Tables to choose from:\n";
		for (int i = 0; i < Database::getTables().size(); i++)
			cout << Database::getTables().at(i).getName() << "\n";
		cout << "\n";
		getline(cin, secondTable);
		if (secondTable.size() == 0 || Database::tableExists(secondTable) == -1)	//empty name or table doesnt exits
			cout << "Please insert a valid table name.\n\n";
		else
			break;
	}

	cout << productTable(newTable, firstTable, secondTable);
}

void Application::updatePrompt()
{
	string tableName;
	while (true)
	{
		printTableList();
		getline(cin, tableName);
		if (tableName.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	if (!Database::tableExists(tableName))
		cout << "Table " << tableName << " doesn't exist.\n\n";
	else
	{
		Table table = Database::getTable(tableName);

		cout << "\nType a column to be manipulated: \n";
		for (int i = 0; i < table.getColNames().size(); i++)
			cout << table.getColNames().at(i) << "\n";
		cout << "\n";

		string columnName;
		getline(cin, columnName);
		
		string value;
		cout << "What value do you want to set " << columnName << " to?: ";
		getline(cin, value);

		string condition;
		cout << "What is the condition where you want " << columnName << " set to " << value << "? (Example: '< 0' without quotes)\n";
		getline(cin, condition);

		cout << updateTable(tableName, columnName, value, condition);
	}
		
}

void Application::deletePrompt()
{
	string tableName;
	while (true)
	{
		printTableList();
		getline(cin, tableName);
		if (tableName.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	if (!Database::tableExists(tableName))
		cout << "Table " << tableName << " doesn't exist.\n\n";
	else
	{
		Table table = Database::getTable(tableName);

		cout << "\nType a column that has the condition to delete from: \n";
		for (int i = 0; i < table.getColNames().size(); i++)
			cout << table.getColNames().at(i) << "\n";
		cout << "\n";

		string columnName;
		getline(cin, columnName);

		string condition;
		cout << "What is the condition of " << columnName << " where you want the entry deleted? (Example: '< 0' without quotes)\n";
		getline(cin, condition);

		cout << deleteFromTable(tableName, columnName, condition);
	}
}

void Application::selectPrompt()
{
	string newTable;
	string tableFrom;
	string columnName;
	string condition;

	//new table name
	while (true)
	{
		cout << "Insert a name for the new table to be created: ";
		getline(cin, newTable);
		if (newTable.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	//first table name
	while (true)
	{
		cout << "\nType a table name to select from. Tables to choose from:\n";
		for (int i = 0; i < Database::getTables().size(); i++)
			cout << Database::getTables().at(i).getName() << "\n";
		cout << "\n";
		getline(cin, tableFrom);
		if (tableFrom.size() == 0 || Database::tableExists(tableFrom) == -1)	//empty name or table doesnt exits
			cout << "Please insert a valid table name.\n\n";
		else
			break;
	}

	Table table = Database::getTable(tableFrom);

	cout << "\nType a column that has the condition to select from: \n";
	for (int i = 0; i < table.getColNames().size(); i++)
		cout << table.getColNames().at(i) << "\n";
	cout << "\n";

	getline(cin, columnName);

	cout << "What is the condition of " << columnName << " where you want the entry selected? (Example: '< 0' without quotes)\n";
	getline(cin, condition);

	cout << tableSelect(newTable, tableFrom, columnName, condition);
}

void Application::renamePrompt()
{

}

void Application::projectPrompt()
{

}

void Application::writePrompt()
{
	string tableName;
	while (true)
	{
		printTableList();
		getline(cin, tableName);
		if (tableName.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	if (Database::tableExists(tableName))
		cout << writeTable(tableName);
	else
		cout << "Table " << tableName << " doesn't exist.\n\n";
}

void Application::openPrompt()
{
	string tableName;
	while (true)
	{
		printTableList();
		getline(cin, tableName);
		if (tableName.size() == 0)
			cout << "Please insert a table name.\n\n";
		else
			break;
	}

	if (Database::tableExists(tableName))
		cout << openTable(tableName);
	else
		cout << "Table " << tableName << " doesn't exist.\n\n";
}


string Application::createTable(string tableName, vector<string> columnNames, vector<string> types)
{
	//following this: CREATE TABLE points (x INTEGER, y INTEGER, z INTEGER) PRIMARY KEY (x, y, z);

	string returnString = "CREATE TABLE " + tableName + " (";
	for (int i = 0; i < columnNames.size(); i++)
		returnString += columnNames.at(i) + " " + types.at(i) + ", ";
	//remove comma at end
	returnString = returnString.substr(0, returnString.size() - 2);
	returnString += ") PRIMARY KEY (";
	for (int i = 0; i < columnNames.size(); i++)
		returnString += columnNames.at(i) + ", ";
	//remove comma at end
	returnString = returnString.substr(0, returnString.size() - 2);
	returnString += ");";
	
	return returnString;
}

string Application::insertIntoTable(string tableName, vector<string> entryValues)
{
	string returnString = "INSERT INTO " + tableName + " VALUES FROM (";
	for (int i = 0; i < entryValues.size(); i++)
		returnString += "\"" + entryValues.at(i) + "\", ";
	//remove comma at end
	returnString = returnString.substr(0, returnString.size() - 2);
	returnString += ");";
	return returnString;
}

string Application::showTable(string tableName)
{
	return "SHOW " + tableName + ";";
}

string Application::openTable(string tableName)
{
	return "OPEN " + tableName + ";";
}

string Application::unionTable(string newTableName, string firstTable, string secondTable)
{
	return newTableName + " <- " + firstTable + " + " + secondTable + ";";
}

string Application::differenceTable(string newTableName, string firstTable, string secondTable)
{
	return newTableName + " <- " + firstTable + " - " + secondTable + ";";
}

string Application::productTable(string newTableName, string firstTable, string secondTable)
{
	return newTableName + " <- " + firstTable + " * " + secondTable + ";";
}

string Application::renameTable(string newTableName, string tableName, vector<string> newAttributes)
{
	string returnString = newTableName + " <- rename (";
	for (int i = 0; i < newAttributes.size(); i++)
		returnString += newAttributes.at(i) + ", ";
	//remove last comma and space
	returnString = returnString.substr(0, returnString.size() - 2);
	returnString += ") " + tableName + ";";

	return returnString;
}

string Application::writeTable(string tableName)
{
	return "WRITE " + tableName + ";";
}

string Application::updateTable(string tableName, string columnName, string setTo, string condition)
{
	return "UPDATE " + tableName + " SET " + columnName + " = " + setTo + " WHERE " + columnName + " " + condition + ";";
}

string Application::deleteFromTable(string tableName, string colName, string condition)
{
	return "DELETE FROM " + tableName + " WHERE (" + colName + " " + condition + ");";
}

string Application::tableSelect(string newTable, string tableFrom, string columnName, string condition)
{
	return newTable + " <- select (" + columnName + " " + condition + ") " + tableFrom + ";";
}

string Application::tableProjection()
{
	return "";
}