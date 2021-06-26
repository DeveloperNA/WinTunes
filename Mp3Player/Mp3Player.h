#pragma once
#include <iostream>
#include <filesystem>
#include <set>
#include "Song.h"
#include "Playlist.h"
#include <vector>
#include <list>

class Mp3Player
{
private:
	
	list<Song>::iterator currentSong;
	void setList(vector<filesystem::directory_entry> mp3files);


public:
	list<Song> songQueue;
	list<Song> songList;
	list<Song> tempQueue;
	Playlist playlist;

	Mp3Player();
	
	list<Song>::iterator setCurrentSong(int id, list<Song>& tlist);
	bool addSongsToQueue(int id, list<Song>& queue);
	bool removeSongsFromQueue(int id, list<Song>& queue);
	bool removeAllSongsFromQueue(list<Song>& queue);


	void play();
	void pause();
	void stop();
	list<Song>::iterator getCurrentSong();
	bool iterateQueue();

	wstring status();

	void printMp3List();
	void printQueue(list<Song> queue);
};

