// class to represent an entry in the database
#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Entry
{
public:
	vector<string> fields;

	Entry();
	~Entry();

};
