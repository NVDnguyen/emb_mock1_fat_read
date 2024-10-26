#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define COLOR_DEFAULT       7   
#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_MAGENTA       5
#define COLOR_BROWN         6
#define COLOR_LIGHT_GRAY    7
#define COLOR_DARK_GRAY     8
#define COLOR_LIGHT_BLUE    9
#define COLOR_LIGHT_GREEN   10
#define COLOR_LIGHT_CYAN    11
#define COLOR_LIGHT_RED     12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15


static inline void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((bgColor << 4) | textColor));
}

// Hàm làm sạch màn hình
static inline void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


static inline void printHeader(const char* title) {
    setColor(COLOR_LIGHT_BLUE, 0);
    printf("====================================\n");
    printf("          %s\n", title);
    printf("====================================\n");
    setColor(COLOR_DEFAULT, 0);
}


static inline void printFooter() {
    printf("\n====================================\n");
    setColor(COLOR_LIGHT_BLUE, 0);
    printf("  Press 0 to clear screen\n");
    printf("  Press Ctrl+C to quit\n");
    setColor(COLOR_DEFAULT, 0);
}

#endif /*CONSOLE_UTILS_H*/ 
