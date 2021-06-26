// Mp3Player.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <tchar.h>
#include <lmcons.h>
#include <conio.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include "Mp3Player.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

// list
// 1.play a song from list  --> select song --> play/pause/stop.
// 2.Create a new Queue.    --> select songs -->add/remove.
// 3. Play from the Queue.  --> traverse queue -->play/pause/stop.
// 4. Create a new Playlist --> select songs --> add/remove.
// 5. Play from playlist --> traverse queue -->play/pause/stop.
// 6. view all playlists --> print names of playlist and its contents.
// 4.Exit


void addSongs(Mp3Player& mp3, list<Song>& queue);
void removeSongs(Mp3Player& mp3, list<Song>& queue);
void removeAll(Mp3Player& mp3, list<Song>& queue);
int playController(Mp3Player& mp3, list<Song>& tlist);

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    Mp3Player mp3;
    json j;
    int num, choice = 1, qchoice, cpchoice, flag = 1;
    string name;

    while (choice) {
        wcout << "\n\n1. Play a song from list\n2. Add songs to Queue\n3.Play Songs from Queue\n4.Create a Playlist\n5.Play from a Playlist\n6.Remove Playlist\n7.Exit" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            system("CLS");

            while (true) {
                mp3.printMp3List();
                wcout << "select the song to be played or press 0 to Mainmenu:";
                cin >> num;
                if (num == 0) {
                    system("CLS");
                    break;

                }
                else {      //playpausestop()
                    
                    mp3.songQueue.clear();
                    mp3.songQueue.assign(mp3.songList.begin(), mp3.songList.end());

                    mp3.setCurrentSong(num, mp3.songQueue);

                    while (true) {
                        system("CLS");

                        mp3.play();
                        if (!playController(mp3, mp3.songQueue))
                            break;
                        if (next(mp3.getCurrentSong(), 1) != mp3.songQueue.end())
                            mp3.setCurrentSong((++mp3.getCurrentSong())->id, mp3.songQueue);
                        else
                            mp3.setCurrentSong(mp3.songQueue.begin()->id, mp3.songQueue);
                    }

                }
                system("CLS");
            }
            break;
        case 2:
            system("CLS");
            // 1. Add song
            // 2. remove song
            // 3. remove all song
            qchoice = 1;
            if (mp3.songQueue.empty())
                wcout << "Queue is Empty. Suggested to add songs..." << endl;
            else
                wcout << "Already Songs Queued...\nYou can add/remove/remove all songs... " << endl;

            while (qchoice) {
                wcout << "1.Add song\n2.Remove song\n3.Remove All songs\n4.Mainmenu" << endl;
                cin >> qchoice;

                switch (qchoice) {
                case 1:
                    mp3.printMp3List();
                    addSongs(mp3, mp3.songQueue);
                    break;
                case 2:
                    removeSongs(mp3, mp3.songQueue);
                    break;
                case 3:removeAll(mp3, mp3.songQueue);
                    break;
                case 4:qchoice = 0;
                system("CLS");

                    break;
                }
            }
            break;

        case 3:
            system("CLS");
            //play songs from Queue..
            //
            if (mp3.songQueue.empty())
                wcout << " Queue is empty" << endl;

            else {
                wcout << "playing songs from Queue..." << endl;
                list<Song>::iterator itr = mp3.songQueue.begin();
                while (true) {
                    if (itr == mp3.songQueue.end())
                        itr = mp3.songQueue.begin();
                    mp3.setCurrentSong(itr->id, mp3.songQueue);

                    wcout << "Enter 0 to mainmenu:" << endl;
                    mp3.play();
                    if (!playController(mp3, mp3.songQueue))
                        break;

                    itr++;
                }
            }
            break;

        case 4:
            // 1. Add songs
            // 2. remove songs
            // 3. remove all songs
            system("CLS");
            cpchoice = 1;
            if (mp3.playlist.empty()) {
                cout << "No Playlist created..." << endl;
            }
            wcout << "Enter the name for playlist: " << endl;
            cin >> name;

            while (cpchoice) {
                wcout << "1.Add song\n2.Remove song\n3.Remove All songs\n4.Ok\n5.Close" << endl;
                cin >> cpchoice;
                mp3.printQueue(mp3.tempQueue);
                switch (cpchoice) {
                case 1:
                    mp3.printMp3List();
                    addSongs(mp3, mp3.tempQueue);
                    break;
                case 2:
                    removeSongs(mp3, mp3.tempQueue);
                    break;
                case 3:removeAll(mp3, mp3.tempQueue);
                    break;
                case 4:cpchoice = 0;
                    system("CLS");

                    break;
                case 5: flag = 0;
                    system("CLS");

                    cpchoice = 0;
                    break;
                }
            }
            if (flag) {
                mp3.playlist.create(name, mp3.tempQueue);
                mp3.playlist.printCurrentPlaylist(name, mp3.tempQueue);
            }
            mp3.tempQueue.clear();
            break;


        case 5:
            system("CLS");
            if (mp3.playlist.empty()) {
                wcout << "No Playlist Created...\nCreate a playlist to play songs from it..." << endl;
            }
            else {
                mp3.playlist.viewAll();
                wcout << "Enter the name of playlist to play songs: " << endl;
                getline(cin >> ws, name);

                mp3.songQueue = mp3.playlist.getSongs(name);
                
                list<Song>::iterator itr = mp3.songQueue.begin();
                while (true) {
                    if (itr == mp3.songQueue.end())
                        itr = mp3.songQueue.begin();
                    mp3.setCurrentSong(itr->id, mp3.songQueue);

                    wcout << "Enter 0 to mainmenu:" << endl;
                    mp3.play();
                    if (!playController(mp3, mp3.songQueue))
                        break;

                    itr++;
                }
            }
            break;
        case 6:
            system("CLS");

            if (mp3.playlist.empty()) {
                wcout << "No Playlist Created...\nSuggested to Create one..." << endl;
            }
            else {
                mp3.playlist.viewAll();
                wcout << "Enter the name of playlist to remove: " << endl;
                getline(cin >> ws, name);
                if (mp3.playlist.remove(name))
                    wcout << "Removed Successfully..." << endl;
                else
                    wcout << "Failed..." << endl;
            }
            break;

        case 7:
            system("CLS");
            choice = 0;
            break;
        }
    }
    
    return 0;
}



void addSongs(Mp3Player& mp3, list<Song>& queue)
{
     int num;
     while (true) {
        wcout << endl;
        if (!queue.empty()) {
            wcout << "Queue :";
            mp3.printQueue(queue);
        }
        wcout << "select the song to add or enter 0 to back: " << endl;
        cin >> num;

        if (num == 0) {
            system("CLS");
            break;
        }
        else {
            if (!mp3.addSongsToQueue(num, queue))
                wcout << "Song already exists in Queue... Try differnt song..." << endl;
        }
     }
}

void removeSongs(Mp3Player& mp3, list<Song>& queue)
{
    int num;
    if (mp3.songQueue.empty()) {
       wcout << "No Songs currently in Queue..." << endl;
        return;
    }

    while (true) {
        if (!queue.empty()) {
            wcout << "Queue :";
            mp3.printQueue(queue);
        }
        wcout << "select the song to remove or enter 0 to back: " << endl;
        cin >> num;

        if (num == 0) {
            system("CLS");
            break;
        }
        else {
            if (!mp3.removeSongsFromQueue(num, queue))
               wcout << "Song doesn't exists in Queue...Try differnt song..." << endl;
        }
    }
}


void removeAll(Mp3Player& mp3, list<Song>& queue)
{
    if (mp3.removeAllSongsFromQueue(queue))
        wcout << "Removed Successfully..." << endl;
    else
        wcout << "No Songs in buffer..." << endl;
}


int playController(Mp3Player& mp3, list<Song>& tlist)
{
    list<Song>::iterator tmp;

    wcout << "\npress 4 to previous song...\npress 5 to pause...\npress 6 to next song...\npress 0 to back..." << endl;
    while (mp3.status() == L"playing" && !_kbhit()) {
        continue;
    }

    while (mp3.status() == L"playing" || mp3.status() == L"paused") {
        switch (_getch()) {
        case '5': mp3.pause();
            wcout << "\npress 0 to back...\npress 2 to resume..." << endl;

            break;
        case '2': mp3.play();
            system("CLS");

            wcout << "\npress 4 to previous song...\npress 5 to pause...\npress 6 to next song...\npress 0 to back..." << endl;

            break;
        case '4': mp3.stop();
            tmp = mp3.getCurrentSong();
            system("CLS");
            if (tmp != tlist.begin()) {
                tmp--;
                mp3.setCurrentSong(tmp->id, tlist);
                mp3.play();
            }
            else {
                mp3.setCurrentSong(tlist.back().id, tlist);
            }
            
            mp3.play();
            

            wcout << "\npress 4 to previous song...\npress 5 to pause...\npress 6 to next song...\npress 0 to back..." << endl;
            break;

        case '6': mp3.stop();/*
            tmp = mp3.getCurrentSong();
            if (++tmp != tlist.end()) {
                mp3.setCurrentSong(tmp->id, tlist);
                mp3.play();
            }
            else {
                cout << "\t----End Of Queue----" << endl;
                cout << "Playing from beginning..." << endl;*/
              //  mp3.setCurrentSong(tlist.begin()->id, tlist);
            //}
            system("CLS");

            wcout << "\npress 4 to previous song...\npress 5 to pause...\npress 6 to next song...\npress 0 to back..." << endl;
            break;

        case '0': mp3.stop();
            system("CLS");

            return 0;
            break;
        }

        while (mp3.status() == L"playing" && !_kbhit()) {
            continue;
        }
        
    }
    
    return 1;
}