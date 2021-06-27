#include "Utils.h"
#include <tchar.h>
#include <lmcons.h>

using namespace std;

vector<wstring> Utils::getDriveList()
{
    TCHAR lpBuffer[200];    // Tchar expands to char if _unicode is defined else wchar_t.
       // In our code utf-16 is used .. so wchar_t will be set.
    TCHAR* driveptr;

    DWORD test = GetLogicalDriveStrings(200, lpBuffer);
    vector<wstring> drives;
    if (test != 0)
    {
        //wcout << "GetLogicalDriveStrings() return value " << test << endl;
        //wcout << "Error if any: " << GetLastError() << endl;
        wcout << "Drives Found: " << endl;
        // the lpBuffer contains all the drives in the system which is of wchar_t type.
        // lpBuffer stores a character in two bytes.
        // C  :  \  '\0'  D  :  \  '\0'  '\0'

        for (driveptr = lpBuffer; *driveptr != '\0'; driveptr += _tcsclen(driveptr) + 1) {
            wstring temp(&driveptr[0]);
            drives.push_back(temp);
        }

    }
    else {
        wcout << "GetLogicalDriveStrings() Failed..:( " << endl;
    }

    for (int i = 0; i < drives.size(); i++) {
        wcout << drives[i] << "\t";
    }
    wcout << endl << endl;
    return drives;
}

vector<wstring> Utils::removeCDDrive(vector<wstring> drives)
{
    for (vector<wstring>::iterator drive = drives.begin(); drive != drives.end(); ) {
        if (DRIVE_CDROM == GetDriveType(drive->c_str())) {
            drive = drives.erase(drive);
            //wcout << "removed CD drive" << endl;
        }
        else {
            drive++;
        }
    }
    return drives;
}

vector<filesystem::directory_entry> Utils::getAllFiles(vector<wstring> drives, string extension)
{
    vector<filesystem::directory_entry> files;
    for (wstring drive : drives) {
        //wcout << drive << endl;
        std::filesystem::recursive_directory_iterator it(drive, std::filesystem::directory_options::skip_permission_denied);
        std::filesystem::recursive_directory_iterator end;  // end of sequence iterator will be set

        for (it; it != end; it++) {
            if (it->path().extension() == extension) { // extension to filter  
                files.push_back(*it);
                std::wcout << "File Found in "<< it->path().c_str()  << std::endl;
            }
        }
    }
    return files;
}

wstring Utils::getCurrentUsername()
{
    TCHAR name[UNLEN + 1];
    DWORD size = UNLEN + 1;

    if (GetUserName((TCHAR*)name, &size))
        wcout << L"Hello, " << name << L"!\n";
    return name;
}

wstring Utils::getShortPathName(const wstring path)
{
    DWORD retval;
    wchar_t* shortpath = new wchar_t[MAX_PATH];
    TCHAR buffer = MAX_PATH;

    retval = GetShortPathName(path.c_str(), shortpath, buffer);

    if (retval == 0) {
        wcout << "GetShortPathName failed " << retval << endl;
    }
   /* else {
        wcout << "Actual path :" << path << endl;
        wcout <<"Short Path : " <<  shortpath << endl;
    }*/
 
    return shortpath;
}