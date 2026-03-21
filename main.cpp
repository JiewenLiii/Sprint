/**
 * @file main.cpp
 * @brief Dungeon Adventure Game Entry Point
 * 
 * Compile:
 * - Windows (MinGW): g++ -std=c++11 -o dungeon.exe main.cpp map.cpp player.cpp enemy.cpp game.cpp
 * - Windows (MSVC): cl /EHsc /std:c++11 main.cpp map.cpp player.cpp enemy.cpp game.cpp
 * - Linux/macOS: g++ -std=c++11 -o dungeon main.cpp map.cpp player.cpp enemy.cpp game.cpp
 */

#include "game.h"
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Game game;
    game.run();

    return 0;
}
