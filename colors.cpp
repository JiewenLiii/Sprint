#include "colors.h"

#ifdef _WIN32
HANDLE Colors::hConsole = nullptr;
WORD Colors::originalAttrs = 0;
bool Colors::initialized = false;

void Colors::init() {
    if (!initialized) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(hConsole, &info)) {
            originalAttrs = info.wAttributes;
        }
        initialized = true;
    }
}

void Colors::set(Color color) {
    init();
    WORD attr = 0;
    switch (color) {
        case COLOR_RED: attr = FOREGROUND_RED; break;
        case COLOR_GREEN: attr = FOREGROUND_GREEN; break;
        case COLOR_YELLOW: attr = FOREGROUND_RED | FOREGROUND_GREEN; break;
        case COLOR_BLUE: attr = FOREGROUND_BLUE; break;
        case COLOR_BRIGHT_RED: attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
        case COLOR_BRIGHT_GREEN: attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case COLOR_BRIGHT_YELLOW: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case COLOR_BRIGHT_BLUE: attr = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        default: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
    }
    SetConsoleTextAttribute(hConsole, attr);
}

void Colors::reset() {
    if (initialized) {
        SetConsoleTextAttribute(hConsole, originalAttrs);
    }
}

#else
// Unix/Linux/macOS 实现
void Colors::init() {}

void Colors::set(Color color) {
    std::cout << "\033[" << static_cast<int>(color) << "m";
}

void Colors::reset() {
    std::cout << "\033[0m";
}
#endif
