/**
 * @file main.cpp
 * @brief Dungeon Adventure Game Entry Point
 */

#include "game.h"
#include "colors.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    Colors::init();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    bool showMenu = true;
    while (showMenu) {
        Game game;
        showMenu = game.run();
    }

    std::cout << std::endl;
    color(COLOR_CYAN);
    std::cout << "Thanks for playing! Goodbye." << std::endl;
    resetColor();

    return 0;
}
