#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define DEFAULT       7   
#define BLACK         0
#define BLUE          1
#define GREEN         2
#define CYAN          3
#define RED           4
#define MAGENTA       5
#define BROWN         6
#define YELLOW        14
#define WHITE         15


static inline void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((bgColor << 4) | textColor));
}

static inline void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


static inline void printHeader(const char* title) {
    setColor(BLUE, 0);
    printf("====================================\n");
    printf("          %s\n", title);
    printf("====================================\n");
    setColor(DEFAULT, 0);
}


static inline void printFooter() {
    printf("\n====================================\n");
    setColor(BLUE, 0);
    printf("  Press 0 to back screen\n");
    printf("  Press Ctrl+C to quit\n");
    setColor(DEFAULT, 0);
}

#endif /*CONSOLE_UTILS_H*/ 
