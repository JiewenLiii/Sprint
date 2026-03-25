#include "colors.h"

#ifdef _WIN32
HANDLE Colors::hConsole = nullptr;
WORD Colors::originalAttrs = 0;
bool Colors::initialized = false;
#endif

void Colors::init() {
#ifdef _WIN32
    if (!initialized) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(hConsole, &info)) {
            originalAttrs = info.wAttributes;
        }
        initialized = true;
    }
#endif
}

void Colors::set(Color color) {
#ifdef _WIN32
    init();
    WORD attr = 0;

    // Map ANSI colors to Windows colors
    switch (color) {
        case COLOR_BLACK: attr = 0; break;
        case COLOR_RED: attr = FOREGROUND_RED; break;
        case COLOR_GREEN: attr = FOREGROUND_GREEN; break;
        case COLOR_YELLOW: attr = FOREGROUND_RED | FOREGROUND_GREEN; break;
        case COLOR_BLUE: attr = FOREGROUND_BLUE; break;
        case COLOR_MAGENTA: attr = FOREGROUND_RED | FOREGROUND_BLUE; break;
        case COLOR_CYAN: attr = FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        case COLOR_WHITE: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        case COLOR_BRIGHT_RED: attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
        case COLOR_BRIGHT_GREEN: attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case COLOR_BRIGHT_YELLOW: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case COLOR_BRIGHT_BLUE: attr = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case COLOR_BRIGHT_MAGENTA: attr = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case COLOR_BRIGHT_CYAN: attr = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case COLOR_BRIGHT_WHITE: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        default: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
    }

    SetConsoleTextAttribute(hConsole, attr);
#else
    // ANSI escape codes for Unix-like systems
    std::cout << "\033[" << color << "m";
#endif
}

void Colors::reset() {
#ifdef _WIN32
    if (initialized) {
        SetConsoleTextAttribute(hConsole, originalAttrs);
    }
#else
    std::cout << "\033[0m";
#endif
}
