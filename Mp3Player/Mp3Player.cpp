#include <tchar.h>
#include <Windows.h>
#include <filesystem>
#include "Mp3Player.h"
#include "Utils.h"
#include <cstdlib>
#include "MMSystem.h"

using namespace std;

#pragma comment(lib, "winmm.lib")

void welcomeText()
{
	wcout << "Welcome to WINTUNES MUSIC PLAYER" << endl << endl;
}

Mp3Player::Mp3Player()
{
	wstring username = Utils::getCurrentUsername();
	welcomeText();

	vector<wstring> cDrive{
		L"C:\\Users\\" + username + L"\\OneDrive\\",
		L"C:\\Users\\" + username + L"\\Downloads\\",
		L"C:\\Users\\" + username + L"\\Documents\\",
		L"C:\\Users\\" + username + L"\\Music\\",
		L"C:\\Users\\" + username + L"\\Videos\\"
	};
	vector<wstring> drives = Utils::getDriveList();
	
	vector<wstring>::iterator it = find(drives.begin(), drives.end(), L"C:\\");
	drives.erase(it);

	drives.insert(drives.end(), cDrive.begin(),	cDrive.end());

	drives = Utils::removeCDDrive(drives);
	cDrive.clear();
	
	vector<filesystem::directory_entry> files = Utils::getAllFiles(drives, ".mp3");
	sort(files.begin(),
		files.end(),
		[](const filesystem::directory_entry& lhs, const filesystem::directory_entry& rhs) {
			return lhs.path().filename() < rhs.path().filename();
		}
	);

	this->setList(files);
}

void Mp3Player::setList(vector<filesystem::directory_entry> mp3files)
{
	int id = 1;

	/*A lambda is an unnamed function that is useful(in actual programming, not theory)
		for short snippets of code that are impossible to reuseand are not worth naming.*/
	auto BytestoMB = [](uintmax_t size) { return (int)(size / (1024.0 * 1024.0) * 100.0) / 100.0; };

	for (filesystem::directory_entry file : mp3files) {
		if(songList.empty() || songList.back().name != file.path().filename() || songList.back().size != BytestoMB(file.file_size()))
			songList.push_back((Song(id++, file.path().filename(), Utils::getShortPathName(file.path().wstring()), BytestoMB(file.file_size()))));
	}

}

list<Song>::iterator Mp3Player::setCurrentSong(int id, list<Song>& tlist)
{
	for (currentSong = tlist.begin(); currentSong != tlist.end(); currentSong++) {
		if (currentSong->id == id) {
			break;
		}
	}

	return currentSong;
}

list<Song>::iterator Mp3Player::getCurrentSong()
{
	return this->currentSong;
}

void Mp3Player::play()
{
	DWORD res;
//	wchar_t err[256];

	list<Song>::iterator it = currentSong;
	wcout.flush();
	wcout << "Currently Playing Song : " << it->name << endl;
	if (it != songQueue.end()) {
		wstring command = L"open \"" + it->path + L"\" type mpegvideo";
		res = mciSendString(command.c_str(), NULL, 0, NULL);

		//if (mciGetErrorString(res, err, 256))
		//	wcout << err << endl;
		command = L"play \"" + it->path + L"\"";
		res = mciSendString(command.c_str(), NULL, 0, NULL);
		//if (mciGetErrorString(res, err, 256))
		//	wcout << err << endl;
	}
}

void Mp3Player::pause()
{
	list<Song>::iterator it = currentSong;

	if (it != songQueue.end()) {
		wstring command = L"pause \"" + it->path + L"\"";
		mciSendString(command.c_str(), NULL, 0, NULL);
	}
}

void Mp3Player::stop()
{
	list<Song>::iterator it = currentSong;

	if (it != songQueue.end()) {
		wstring command = L"stop \"" + it->path + L"\"";
		mciSendString(command.c_str(), NULL, 0, NULL);
		wstring closecommand = L"close \"" + it->path + L"\"";
		mciSendString(closecommand.c_str(), NULL, 0, NULL);
	}
}


wstring Mp3Player::status()
{
	list<Song>::iterator it = currentSong;

	if (it != songQueue.end()) {
		wstring command = L"status \"" + it->path + L"\" mode";
		wchar_t response[256];

		mciSendString(command.c_str(), response, 256, NULL);
		return response;		
	}
	else {
		return L"failed";
	}
}


void Mp3Player::printMp3List()
{
	wcout << "Songs in the Computer are " << endl;
	list<Song>::iterator itr;
	wcout << "S. No\t Song Name" << endl;
	wcout.flush();
	for (itr = songList.begin(); itr != songList.end(); itr++) {
		wcout << itr->id << "\t" << itr->name << endl;
		
	}
}
 
bool Mp3Player::addSongsToQueue(int id, list<Song>& queue)
{
	for (list<Song>::iterator itr = queue.begin(); itr != queue.end(); itr++) {
		if (itr->id == id)
			return false;
	}
	queue.push_back(*this->setCurrentSong(id, this->songList));
	return true;
}

bool Mp3Player::removeSongsFromQueue(int id, list<Song>& queue)
{
	list<Song>::iterator temp;
	if (queue.empty())
		return false;
	else
	{
		for (list<Song>::iterator itr = queue.begin(); itr != queue.end(); itr++) {
			if (itr->id == id) {
				temp = itr++;
				queue.erase(temp);
				if (itr != queue.end())
					this->setCurrentSong(itr->id, queue);
				else
					this->setCurrentSong(1, songList);

				return true;
			}
		}

	}
	return false;
}

bool Mp3Player::removeAllSongsFromQueue(list<Song>& queue)
{

	if (queue.empty())
		return false;
	else {
		queue.clear();
		return true;
	}
}


bool Mp3Player::iterateQueue()
{
	/*static deque<Song>::iterator itr = songQueue.begin();
	this->setCurrentSong(itr->id);
	itr++;
	if (itr == songQueue.end())
		return false;
	return true;*/
	static list<Song>::iterator itr = songQueue.begin();
	
	if (!songQueue.empty()) {
		if(itr != songQueue.end())
			cout << itr->id << endl;
		this->setCurrentSong(itr->id, this->songQueue);
		itr++;
		if (itr == songQueue.end())
			itr = songQueue.begin();
		
		return true;
	}
	else
		return false;
}


void Mp3Player::printQueue(list<Song> queue)
{
	for (list<Song>::iterator itr = queue.begin(); itr != queue.end(); itr++) {
		wcout << itr->id << "   ";
	}

	wcout << endl;
}