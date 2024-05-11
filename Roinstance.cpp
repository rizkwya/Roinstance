#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

bool isProcessRunning(const wchar_t* processName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating snapshot." << std::endl;
        return false;
    }

    if (Process32First(snapshot, &entry)) {
        do {
            if (wcscmp(entry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return false;
}

void killProcess(const wchar_t* processName) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating process snapshot." << std::endl;
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hProcessSnap, &pe32)) {
        do {
            if (wcscmp(pe32.szExeFile, processName) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    break;
                }
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);
}

void runScript() {
    CreateMutex(NULL, TRUE, L"ROBLOX_singletonMutex");
    Sleep(-1);
}

int main() {
    system("title Roinstance");
    const wchar_t* processName = L"RobloxPlayerBeta.exe";

    while (true) {
        if (isProcessRunning(processName)) {
            killProcess(processName);
        }
        else {
            runScript();
        }

        Sleep(1000);
    }

    return 0;
}


