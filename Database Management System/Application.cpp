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


	cout << "\n\n";
	system("PAUSE");
}

void Application::createTable(string tableName, vector<string> columnNames)
{
	Database::addTable(Table(tableName, columnNames));
}

Table Application::insertIntoTable(string tableName, Entry& e)
{
	Database::getTable(tableName).addEntry(e);
}

void Application::showTable(string tableName)
{
	Database::getTable(tableName).printTable();
}

Table Application::unionTable(string firstTable, string secondTable)
{
	return Database::setunion(Database::getTable(firstTable), Database::getTable(secondTable));
}

Table Application::differenceTable(string firstTable, string secondTable)
{
	return Database::differenceTable(Database::getTable(firstTable), Database::getTable(secondTable));
}

Table Application::productTable(string firstTable, string secondTable)
{
	return Database::productTable(Database::getTable(firstTable), Database::getTable(secondTable));
}

void Application::updateTable(string tableName)
{
}

void Application::deleteFromTable(string tableName)
{
}

void Application::tableSelect(string tableName)
{
}

void Application::renameTable(string tableName)
{
}

void Application::writeTable(string tableName)
{
}

void Application::tableProjection()
{
}