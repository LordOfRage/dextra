#include <iostream>
#include "getch.hpp"

#if defined(WIN32) || defined(_WIN32)

#include <windows.h>

char dextra_getch(){
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT) & (~ENABLE_LINE_INPUT));

    char data;
    ReadFile(hStdin, &data, 1, NULL, nullptr);

    return data;
}


#else

#include <unistd.h>
#include <termios.h>

char dextra_getch(){
    termios TermConf;
    tcgetattr(STDIN_FILENO, &TermConf);

    TermConf.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &TermConf);

    char data;
    data = std::cin.get();

    return data;
}

#endif // WIN32
