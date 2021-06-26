#pragma once

#include <iostream>
#include "Song.h"
#include <list>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

class Playlist
{
private:
	json obj;

	void loadJsonObj();
	void writeJsonObj();

public:
	list<Song> tempQueue;

	Playlist();

	void create(string name, list<Song>& list);
	bool remove(string name);
	bool empty();
	list<Song> getSongs(string name);

	void printCurrentPlaylist(string name, list<Song> queue);
	void viewAll();
};

