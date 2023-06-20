#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <wininet.h>

// Функция отправки HTTP-запроса
bool SendHttpRequest(const std::string& url)
{
    HINTERNET hInternet = InternetOpenA("HTTP Request", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        std::cout << "Error initializing HTTP session!" << std::endl;
        return false;
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL)
    {
        std::cout << "Failed to send HTTP query!" << std::endl;
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[4096];
    DWORD bytesRead;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
    {
        // Пропускаем полученные данные
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return true;
}

int main()
{
    const char* title = "Locomotive lights app for ZDSimulator 55.008 - v.0.1"; // Замените на нужный заголовок

    if (!SetConsoleTitleA(title))
    {
        std::cout << "Error setting window title." << std::endl;
    }

    std::cout << "Hello! This program is designed to work only with ZDSimulator version 55.008.\nIt provides transmission of readings of the locomotive lights\nof the game to special equipment via the HTTP protocol.\nUse with OrangePI board and relays module.\nAll instructions you can found in my GitHub.\nAuthor: UR3ZLT aka thescs (https://github.com/thescs)\n\n" << std::endl;
    // Получаем идентификатор процесса Launcher.exe
    DWORD launcherProcessId = 0;

    // Создаем снимок процессов
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32W pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32W);

        // Ищем процесс Launcher.exe в снимке
        if (Process32FirstW(hSnapshot, &pe32))
        {
            do
            {
                if (wcscmp(pe32.szExeFile, L"Launcher.exe") == 0)
                {
                    launcherProcessId = pe32.th32ProcessID;
                    std::cout << "Found process Launcher.exe with PID: " << launcherProcessId << std::endl;
                    break;
                }
            } while (Process32NextW(hSnapshot, &pe32));
        }

        // Закрываем снимок процессов
        CloseHandle(hSnapshot);
    }

    if (launcherProcessId == 0)
    {
        std::cout << "Process Launcher.exe not found. Make sure you launched the game." << std::endl;
        return 1;
    }

    // Оффсет искомых данных
    const uintptr_t offset = 0x09007ECC;

    // Открываем процесс с правами доступа для чтения памяти
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, launcherProcessId);
    if (hProcess == NULL)
    {
        std::cout << "Couldnt open process Launcher.exe." << std::endl;
        return 1;
    }

    // Буфер для хранения предыдущего значения
    int prevValue = 0;

    while (true)
    {
        // Считываем текущее значение из памяти
        int currValue = 0;
        ReadProcessMemory(hProcess, (LPVOID)offset, &currValue, sizeof(int), nullptr);

        // Проверяем наличие изменений
        if (currValue != prevValue)
        {
            std::cout << "Changes detected, code: " << currValue << std::endl;

            // Формируем полный URL с новым значением
            std::string url = "http://192.168.88.74:8000/" + std::to_string(currValue);

            // Отправляем HTTP-запрос
            if (!SendHttpRequest(url))
            {
                std::cout << "Failed to send HTTP query!" << std::endl;
            }
        }

        // Обновляем предыдущее значение
        prevValue = currValue;

        // Задержка перед следующей проверкой
        Sleep(100);
    }

    // Закрываем хэндл процесса
    CloseHandle(hProcess);

    return 0;
}
