// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>
#include <shellapi.h>
using namespace std;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
std::wstring s2ws(const std::string& s);

void __stdcall PluginStart(void* aOwner) {
    string line;
    ifstream myfile("plugins\\run_on_open.cfg");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {

            if (line.substr(0, 1).compare("#") != 0 && line.length() > 0) {
                ShellExecute(NULL, L"open", s2ws(line).c_str(),
                    L"",
                    NULL, SW_SHOWNORMAL);
            }
        }
        myfile.close();
    }

    else cout << "Unable to open file";
}


void __stdcall AccessVariable(unsigned short varindex, float* value, bool* write) {
}

void __stdcall AccessSystemVariable(unsigned short varindex, float* value, bool* write) {
}

void __stdcall AccessStringVariable(unsigned short varindex, LPCWSTR* str, bool* write) {
}

void __stdcall AccessTrigger(unsigned short triggerindex, bool* active) {
}

void __stdcall PluginFinalize() {
    string line;
    ifstream myfile("plugins\\run_on_close.cfg");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {

            if (line.substr(0,1).compare("#")!=0 && line.length() > 0) {
                ShellExecute(NULL, L"open", s2ws(line).c_str(),
                    L"",
                    NULL, SW_SHOWNORMAL);
            }
        }
        myfile.close();
    }

    else cout << "Unable to open file";
}

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    std::wstring r(len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
    return r;
}
