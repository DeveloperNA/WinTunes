#pragma once

#include <iostream>
#include <string>
using namespace std;
class Song
{
public:
	int id;
	wstring name;
	wstring path;
	double size;


	Song(int id, wstring name, wstring path, double size);
	void viewInfo();
	
	bool operator<(const Song& rhs) const {
		if (name == rhs.name && size == rhs.size) {
			return false;
		}
		return id < rhs.id;
	}


};

