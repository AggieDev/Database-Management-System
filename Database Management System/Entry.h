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
	Entry(vector<string> vec){ fields = vec; };
	~Entry();

	void printEntry();
<<<<<<< HEAD
	vector<string> getFields(){ return fields; }
};
=======
	vector<string> getFields() const { return fields; }
	string operator[](int i) { return fields[i]; }
};

ostream& operator<<(ostream& out, const Entry& e);
>>>>>>> origin/master
