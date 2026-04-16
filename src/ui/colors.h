#ifndef COLORS_H
#define COLORS_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>

// Color codes (cross-platform)
enum Color {
    COLOR_RESET = 0,
    COLOR_BLACK = 30,
    COLOR_RED = 31,
    COLOR_GREEN = 32,
    COLOR_YELLOW = 33,
    COLOR_BLUE = 34,
    COLOR_MAGENTA = 35,
    COLOR_CYAN = 36,
    COLOR_WHITE = 37,
    COLOR_BRIGHT_RED = 91,
    COLOR_BRIGHT_GREEN = 92,
    COLOR_BRIGHT_YELLOW = 93,
    COLOR_BRIGHT_BLUE = 94,
    COLOR_BRIGHT_MAGENTA = 95,
    COLOR_BRIGHT_CYAN = 96,
    COLOR_BRIGHT_WHITE = 97,

    // Background colors
    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BG_CYAN = 46,
    BG_WHITE = 47
};

class Colors {
public:
    static void init();
    static void set(Color color);
    static void reset();
    
private:
#ifdef _WIN32
    static HANDLE hConsole;
    static WORD originalAttrs;
    static bool initialized;
#endif
};

// Inline functions for convenience
inline void color(Color c) { Colors::set(c); }
inline void resetColor() { Colors::reset(); }

// RAII color scope guard
class ColorScope {
public:
    ColorScope(Color c) { Colors::set(c); }
    ~ColorScope() { Colors::reset(); }
};

#endif // COLORS_H
