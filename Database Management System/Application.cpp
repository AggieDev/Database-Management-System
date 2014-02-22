#pragma once
#include "Application.h"


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


	cout << "\n\n";
	system("PAUSE");
}

string Application::createTable(string tableName, vector<string> columnNames, vector<char> types)
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

string Application::insertIntoTable(string tableName, Entry& e)
{
	string returnString = "INSERT INTO " + tableName + " VALUES FROM (";
	for (int i = 0; i < e.getFields().size(); i++)
		returnString += "\"" + e.getFields().at(i) + "\", ";
	//remove comma at end
	returnString = returnString.substr(0, returnString.size() - 2);
	returnString += ");";
	return "";
}

string Application::showTable(string tableName)
{
	return "SHOW " + tableName + ";";
}

string Application::unionTable(string newTableName, string firstTable, string secondTable)
{
	return newTableName + " <- " + firstTable + " + " + secondTable;
}

string Application::differenceTable(string newTableName, string firstTable, string secondTable)
{
	return newTableName + " <- " + firstTable + " - " + secondTable;
}

string Application::productTable(string newTableName, string firstTable, string secondTable)
{
	return newTableName + " <- " + firstTable + " * " + secondTable;
}

string Application::updateTable(string tableName, string key, string newVal, int keyCol, int valCol)
{
	return "";
}

string Application::deleteFromTable(string tableName)
{
	return "";
}

string Application::tableSelect(string tableName)
{
	return "";
}

string Application::renameTable(string tableName)
{
	return "";
}

string Application::writeTable(string tableName)
{
	return "";
}

string Application::tableProjection()
{
	return "";
}