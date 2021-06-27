#include "Playlist.h"

Playlist::Playlist()
{
	this->loadJsonObj();
}

void Playlist::create(string name, list<Song>& tlist)
{
	for (list<Song>::iterator itr = tlist.begin(); itr != tlist.end(); itr++) {
		this->obj[name].push_back({
			{ "id", itr->id }, 
			{ "Name", itr->name }, 
			{ "Path",itr->path }, 
			{ "Size", itr->size }
		});
	}
	this->writeJsonObj();

}

bool Playlist::empty()
{
	ifstream in("app.json", ifstream::in);
	if (in.is_open() && in.peek() != ifstream::traits_type::eof()) {
		if (this->obj.empty())
			return true;

		return false;
	}


	return true;
}

list<Song> Playlist::getSongs(string name)
{
	list<Song> queue;
	json::iterator element = this->obj.find(name);
	if (element != this->obj.end()) {
		json temp = element.value();
		for (json::iterator itr = temp.begin(); itr != temp.end(); itr++) {
			queue.push_back(
				Song(
					itr->at("id"),
					itr->at("Name"),
					itr->at("Path"),
					itr->at("Size")
				)
			);
		}
	}
	
	return queue;
}

bool Playlist::remove(string name)
{
	json::iterator playlist = this->obj.find(name);
	if (playlist != this->obj.end()) {
		this->obj.erase(playlist);
		this->writeJsonObj();
		return true;
	}
	return false;
}

void Playlist::loadJsonObj()
{
	ifstream in("app.json", ifstream::in);
	if (in.is_open() && in.peek() != ifstream::traits_type::eof()) {
		in >> this->obj;
	}
}

void Playlist::writeJsonObj()
{
	ofstream out("app.json", ofstream::out);
	out << setw(2) <<  this->obj << endl;
}

void Playlist::viewAll()
{
	list<Song> queue;
	string playlist;

	if (!this->obj.empty()) {
		for (json::iterator itr = this->obj.begin(); itr != this->obj.end(); itr++) {
			json values = itr.value();
			playlist = itr.key();
			wcout << "Playlist : " << playlist.c_str() << endl;
			wcout << "\nId" << "\t" << "Name" << endl;

			for (json::iterator jtr = values.begin(); jtr != values.end(); jtr++) {
				int id = jtr->at("id");
				wstring name = jtr->at("Name");
				wcout <<  id << "\t";

				wcout <<  name << endl;
				
			}
			wcout << "\n------------------------------------------------\n";
		}
	}
	else {
		wcout << "Currently No Playlist to display..." << endl;
	}
}

void Playlist::printCurrentPlaylist(string name, list<Song> queue)
{
	wcout << "Playlist : " << name.c_str() << endl;
	wcout << endl;
	wcout << "Id" << "\t" << "Name" << endl;
	for (list<Song>::iterator itr = queue.begin(); itr != queue.end(); itr++) {
		wcout << itr->id << "\t" << itr->name << endl;
	}

	wcout << "\n------------------------------------------------\n";
}