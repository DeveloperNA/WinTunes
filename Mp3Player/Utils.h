#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>
#include <filesystem>

using namespace std;
// A namespace is simply an abstract way of grouping items together

namespace Utils
{
	wstring getCurrentUsername();
	wstring getShortPathName(wstring path);
	vector<wstring> getDriveList();
	vector<wstring> removeCDDrive(vector<wstring> drives);
	vector<filesystem::directory_entry> getAllFiles(vector<wstring> drives, string extension);
}

