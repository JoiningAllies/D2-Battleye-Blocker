#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <filesystem>
#include <lmcons.h>
#include <ctime>

namespace fs = std::filesystem;

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void deleteFile(const std::string& filename, bool& deleted) {
    if (fs::exists(filename)) {
        fs::remove(filename);
        deleted = true;
    }
}

void blockInternetTraffic(const std::string& programPath) {
    std::string command = "netsh advfirewall firewall add rule name=\"Block Program\" dir=out program=\"" + programPath + "\" action=block >nul 2>&1";
    system(command.c_str());
    command = "netsh advfirewall firewall add rule name=\"Block Program\" dir=in program=\"" + programPath + "\" action=block >nul 2>&1";
    system(command.c_str());
}

std::wstring getTempFolderPath() {
    wchar_t username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameW(username, &username_len);
    std::wstring tempFolderPath = L"C:\\Users\\" + std::wstring(username) + L"\\AppData\\Local\\Temp\\Destiny2";
    return tempFolderPath;
}

void deleteFolderContents(const std::wstring& folderPath, int& foldersDeleted, int& filesDeleted) {
    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        return;
    }
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (fs::is_directory(entry)) {
            fs::remove_all(entry.path());
            foldersDeleted++;
        }
        else if (fs::is_regular_file(entry)) {
            fs::remove(entry.path());
            filesDeleted++;
        }
    }
}

void setFontSize(int size) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    fontInfo.dwFontSize.Y = size;
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

void removeConsoleBorders() {
    HWND hwnd = GetConsoleWindow();
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    SetWindowLong(hwnd, GWL_STYLE, style);
    SetWindowPos(hwnd, NULL, 0, 0, 610, 180, SWP_NOZORDER | SWP_FRAMECHANGED);
}

std::string generateRandomTitle(int totalLength) {
    const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string title;

    const std::string word1 = "Velocity";
    const std::string word2 = "MadeByRikka";
    int word1Length = word1.length();
    int word2Length = word2.length();

    int position1 = rand() % (totalLength - word1Length - word2Length);
    int position2 = rand() % (totalLength - word1Length - word2Length);

    if (position1 > position2) std::swap(position1, position2);

    for (int i = 0; i < position1; ++i) {
        title += characters[rand() % characters.size()];
    }

    title += word1;

    for (int i = position1 + word1Length; i < position2; ++i) {
        title += characters[rand() % characters.size()];
    }

    title += word2;

    for (int i = position2 + word2Length; i < totalLength; ++i) {
        title += characters[rand() % characters.size()];
    }

    return title;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    std::string randomTitle = generateRandomTitle(70);
    SetConsoleTitleA(randomTitle.c_str());

    removeConsoleBorders();
    setFontSize(16);

    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::string exeDir = fs::path(exePath).parent_path().string();

    std::string destinyExePath = exeDir + "\\Destiny2.exe";
    if (!fs::exists(destinyExePath)) {
        std::cout << "Please make sure you put me in the same folder as your Destiny2.exe" << std::endl;
        Sleep(3000);
        return 0;
    }

    std::string fileToDelete = exeDir + "\\miniTicketDbg.exe";
    bool fileDeleted = false;
    deleteFile(fileToDelete, fileDeleted);

    if (fileDeleted) {
        std::cout << "Ticket creator deleted" << std::endl;
    }
    else {
        std::cout << "Ticket creator has already been deleted" << std::endl;
    }

    std::cout << std::endl;

    std::string programToBlock = exeDir + "\\battleye\\BEService_x64.exe";
    blockInternetTraffic(programToBlock);

    setColor(10);
    std::cout << "Battleye connection blocked" << std::endl;
    setColor(7);

    std::cout << std::endl;

    std::wstring folderToDelete = getTempFolderPath();
    int foldersDeleted = 0;
    int filesDeleted = 0;
    deleteFolderContents(folderToDelete, foldersDeleted, filesDeleted);

    int totalDeleted = foldersDeleted + filesDeleted;
    if (totalDeleted == 0) {
        setColor(10);
    }
    else if (totalDeleted >= 1 && totalDeleted <= 4) {
        setColor(14);
    }
    else {
        setColor(12);
    }
    std::cout << "Reports Deleted: " << totalDeleted << std::endl;
    setColor(7);

    std::cout << std::endl;

    std::cout << "Only close me after you are done playing Destiny so Battleye stays Blocked <3" << std::endl;

    while (true) {
        Sleep(1000);
    }
    return 0;
}

// Made by Rikka 7/13/24 <3