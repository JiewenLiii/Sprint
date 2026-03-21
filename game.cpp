#include "game.h"
#include <iostream>
#include <string>
#include <cctype>

Game::Game() : isRunning(true), isVictory(false) {
    spawnEnemies(map, enemies, ENEMY_COUNT, player.x, player.y);
}

void Game::run() {
    std::cout << "================================" << std::endl;
    std::cout << "   Welcome to Dungeon Adventure!" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Objective: Defeat all enemies!" << std::endl;
    std::cout << "Controls: W/A/S/D to move, Q to quit" << std::endl;
    std::cout << "Press Enter to start..." << std::endl;
    
    std::string dummy;
    std::getline(std::cin, dummy);

    while (isRunning) {
        render();
        showPlayerStatus();

        if (!handleInput()) {
            break;
        }

        updateEnemies();
        checkGameOver();
    }

    showGameOver();
}

void Game::render() {
    int enemyPositions[10][2];
    int count = 0;

    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && count < 10) {
            enemyPositions[count][0] = enemy.x;
            enemyPositions[count][1] = enemy.y;
            count++;
        }
    }

    map.render(player.x, player.y, enemyPositions, count);
}

void Game::showPlayerStatus() {
    std::cout << std::endl;
    std::cout << "Player Status: HP " << player.hp << "/" << player.maxHp 
              << " | Attack: " << player.attack << std::endl;
    std::cout << "Enemies Remaining: " << countAliveEnemies() << std::endl;
}

int Game::countAliveEnemies() const {
    int count = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            count++;
        }
    }
    return count;
}

bool Game::handleInput() {
    std::cout << std::endl << "Enter action (W/A/S/D/Q): ";
    
    char input;
    std::cin >> input;
    input = static_cast<char>(std::toupper(static_cast<unsigned char>(input)));

    int dx = 0, dy = 0;
    bool moved = false;

    switch (input) {
        case 'W':
            dy = -1;
            moved = player.move(dx, dy, map);
            break;
        case 'S':
            dy = 1;
            moved = player.move(dx, dy, map);
            break;
        case 'A':
            dx = -1;
            moved = player.move(dx, dy, map);
            break;
        case 'D':
            dx = 1;
            moved = player.move(dx, dy, map);
            break;
        case 'Q':
            isRunning = false;
            return false;
        default:
            std::cout << "Invalid input! Use W/A/S/D to move, Q to quit." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            return true;
    }

    if (moved) {
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (enemies[i].isAlive() && 
                enemies[i].x == player.x && 
                enemies[i].y == player.y) {
                combat(i);
                break;
            }
        }
    } else {
        std::cout << "Cannot move to that position!" << std::endl;
    }

    return true;
}

void Game::updateEnemies() {
    if (!player.isAlive()) {
        return;
    }

    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) {
            continue;
        }

        int distX = std::abs(player.x - enemy.x);
        int distY = std::abs(player.y - enemy.y);

        if (distX <= 3 && distY <= 3) {
            enemy.moveTowardsPlayer(player.x, player.y, map);
        } else {
            enemy.randomMove(map);
        }

        if (enemy.x == player.x && enemy.y == player.y) {
            for (size_t i = 0; i < enemies.size(); ++i) {
                if (&enemies[i] == &enemy) {
                    combat(i);
                    break;
                }
            }
        }
    }
}

void Game::combat(size_t enemyIndex) {
    Enemy& enemy = enemies[enemyIndex];

    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "   Combat Started!" << std::endl;
    std::cout << "================================" << std::endl;

    while (enemy.isAlive() && player.isAlive()) {
        int playerDamage = player.attack;
        enemy.takeDamage(playerDamage);
        showCombatLog("You attacked the enemy, dealt " + std::to_string(playerDamage) + " damage");

        if (!enemy.isAlive()) {
            showCombatLog("Enemy defeated!");
            break;
        }

        int enemyDamage = enemy.attack;
        player.takeDamage(enemyDamage);
        showCombatLog("Enemy attacked you, dealt " + std::to_string(enemyDamage) + " damage");

        if (!player.isAlive()) {
            showCombatLog("You were defeated...");
            break;
        }

        std::cout << "  Enemy HP: " << enemy.hp << "/" << enemy.maxHp << std::endl;
        std::cout << "  Player HP: " << player.hp << "/" << player.maxHp << std::endl;
    }

    std::cout << "================================" << std::endl;
    std::cout << "Press Enter to continue..." << std::endl;
    std::string dummy;
    std::getline(std::cin, dummy);
}

void Game::showCombatLog(const std::string& message) {
    std::cout << "  [Combat] " << message << std::endl;
}

void Game::checkGameOver() {
    if (!player.isAlive()) {
        isRunning = false;
        isVictory = false;
    } else if (countAliveEnemies() == 0) {
        isRunning = false;
        isVictory = true;
    }
}

void Game::showGameOver() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "================================" << std::endl;
    if (isVictory) {
        std::cout << "   Congratulations! You Won!" << std::endl;
        std::cout << "   All enemies have been defeated!" << std::endl;
    } else {
        std::cout << "   Game Over" << std::endl;
        std::cout << "   You were defeated..." << std::endl;
    }
    std::cout << "================================" << std::endl;
    std::cout << "Thanks for playing!" << std::endl;
}
