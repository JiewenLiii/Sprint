#include "game.h"
#include "colors.h"
#include "keyboard.h"
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <chrono>
#include <thread>

Game::Game() : isRunning(true), result(QUIT), difficulty(EASY) {
    Colors::init();
}

bool Game::confirmQuit() {
    std::cout << std::endl;
    color(COLOR_YELLOW);
    std::cout << "  Are you sure you want to quit? (y/n): ";
    resetColor();

    char input = Keyboard::getKey();
    input = static_cast<char>(std::toupper(static_cast<unsigned char>(input)));

    return (input == 'Y');
}

bool Game::showMainMenu() {
    renderer.clearScreen();

    std::cout << std::endl;
    renderer.drawLine(60);
    color(COLOR_BRIGHT_YELLOW);
    renderer.drawCenteredText("⚔  DUNGEON ADVENTURE  ⚔", 60);
    resetColor();
    color(COLOR_WHITE);
    renderer.drawCenteredText("Explore the dungeon, defeat all enemies!", 60);
    resetColor();
    std::cout << std::endl;
    renderer.drawLine(60);
    std::cout << std::endl;

    // 显示难度选项 - 紧凑布局
    color(COLOR_GREEN);
    std::cout << "  [1] EASY MODE" << std::endl;
    resetColor();
    color(COLOR_WHITE);
    std::cout << "      Enemies: 4  |  HP: 10  |  ATK: 3" << std::endl;
    resetColor();
    std::cout << std::endl;

    color(COLOR_RED);
    std::cout << "  [2] HARD MODE" << std::endl;
    resetColor();
    color(COLOR_WHITE);
    std::cout << "      Enemies: 8  |  HP: 15  |  ATK: 6" << std::endl;
    resetColor();
    std::cout << std::endl;

    color(COLOR_CYAN);
    std::cout << "  [3] QUIT GAME" << std::endl;
    resetColor();
    std::cout << std::endl;

    renderer.drawLine(60);
    std::cout << std::endl;

    // 输入容错处理
    char input;
    bool validInput = false;

    while (!validInput) {
        color(COLOR_BRIGHT_CYAN);
        std::cout << "  Select difficulty (1/2/3): ";
        resetColor();

        input = Keyboard::getKey();

        if (input == '1' || input == '2') {
            validInput = true;
            difficulty = (input == '1') ? EASY : HARD;
            settings = getDifficultySettings(difficulty);

            std::cout << std::endl;
            if (difficulty == EASY) {
                color(COLOR_GREEN);
                std::cout << "  >> Starting EASY MODE...";
            } else {
                color(COLOR_RED);
                std::cout << "  >> Starting HARD MODE...";
            }
            resetColor();
            std::cout << std::endl;

        } else if (input == '3') {
            // 退出确认
            if (confirmQuit()) {
                return false;  // 确认退出
            }
            // 取消退出，继续循环
            std::cout << std::endl;
            color(COLOR_YELLOW);
            std::cout << "  >> Quit cancelled. Returning to menu..." << std::endl;
            resetColor();
            std::cout << std::endl;
            renderer.drawLine(60);
            std::cout << std::endl;

        } else {
            // 无效输入
            std::cout << std::endl;
            color(COLOR_BRIGHT_RED);
            std::cout << "  >> Invalid choice. Please select 1, 2, or 3." << std::endl;
            resetColor();
            std::cout << std::endl;
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    return true;
}

bool Game::run() {
    if (!showMainMenu()) {
        return false;
    }

    initGame();

    renderer.clearScreen();
    std::cout << std::endl;
    renderer.drawLine(50);
    color(COLOR_BRIGHT_GREEN);
    renderer.drawCenteredText("GAME START!", 50);
    resetColor();
    renderer.drawLine(50);
    std::cout << std::endl;

    color(COLOR_CYAN);
    std::cout << "  Legend: ";
    resetColor();
    color(COLOR_BRIGHT_GREEN); std::cout << "@=You"; resetColor(); std::cout << ", ";
    color(COLOR_RED); std::cout << "E=Enemy"; resetColor();
    std::cout << std::endl;

    color(COLOR_CYAN);
    std::cout << "          ";
    resetColor();
    std::cout << ".=floor, ";
    color(COLOR_BRIGHT_BLUE); std::cout << "#=wall"; resetColor(); std::cout << ", ";
    color(COLOR_BLACK); std::cout << "?=unknown"; resetColor(); std::cout << ", ";
    color(COLOR_YELLOW); std::cout << ",=footprint"; resetColor();
    std::cout << std::endl;

    std::cout << "  Controls: W/A/S/D to move, Q to quit" << std::endl;
    std::cout << "  Defeat all " << settings.enemyCount << " enemies!" << std::endl;
    std::cout << std::endl;
    color(COLOR_YELLOW);
    std::cout << "  Press any key to start...";
    resetColor();
    Keyboard::getKey();
    std::cout << std::endl;

    while (isRunning) {
        renderer.renderGame(map, player, enemies);
        showPlayerStatus();

        if (!handleInput()) {
            result = QUIT;
            break;
        }

        updateEnemies();
        checkGameOver();
    }

    return showEndScreen();
}

void Game::initGame() {
    player.reset();
    spawnEnemies(map, enemies, settings.enemyCount,
                 settings.enemyHp, settings.enemyAttack,
                 player.getX(), player.getY());
    combatLog.clear();
    isRunning = true;
    result = QUIT;
}

void Game::render() {
    renderer.renderGame(map, player, enemies);
}

void Game::showPlayerStatus() {
    // 委托给 Renderer 显示玩家状态（包括敌人列表）
    renderer.renderPlayerStatus(player, countAliveEnemies(), settings.enemyCount, combatLog, &enemies);
}

int Game::countAliveEnemies() const {
    int count = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) count++;
    }
    return count;
}

bool Game::handleInput() {
    color(COLOR_BRIGHT_CYAN);
    std::cout << "  > Action (W/A/S/D/Q): ";
    resetColor();

    char input = Keyboard::getKey();
    input = static_cast<char>(std::toupper(static_cast<unsigned char>(input)));

    int dx = 0, dy = 0;
    bool moved = false;

    switch (input) {
        case 'W': dy = -1; moved = player.move(dx, dy, map, enemies); break;
        case 'S': dy = 1; moved = player.move(dx, dy, map, enemies); break;
        case 'A': dx = -1; moved = player.move(dx, dy, map, enemies); break;
        case 'D': dx = 1; moved = player.move(dx, dy, map, enemies); break;
        case 'Q': return false;
        default:
            std::cout << "  Use W/A/S/D to move, Q to quit." << std::endl;
            return true;
    }

    if (moved) {
        // 检测是否邻接敌人
        bool adjacentToEnemy = false;
        size_t enemyIndex = 0;
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (enemies[i].isAlive()) {
                int distX = std::abs(player.getX() - enemies[i].getX());
                int distY = std::abs(player.getY() - enemies[i].getY());
                if (distX + distY == 1) {
                    adjacentToEnemy = true;
                    enemyIndex = i;
                    break;
                }
            }
        }

        if (adjacentToEnemy) {
            render();
            showPlayerStatus();
            std::cout << std::endl;
            color(COLOR_BRIGHT_RED);
            std::cout << "Enemy adjacent! Engaging combat..." << std::endl;
            resetColor();

            combat(enemyIndex, false);
        }
    } else {
        std::cout << "  Cannot move there!" << std::endl;
    }

    return true;
}

void Game::updateEnemies() {
    if (!player.isAlive()) return;

    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy& enemy = enemies[i];
        if (!enemy.isAlive()) continue;

        int distX = std::abs(player.getX() - enemy.getX());
        int distY = std::abs(player.getY() - enemy.getY());

        if (distX <= CHASE_RADIUS && distY <= CHASE_RADIUS) {
            enemy.moveTowardsPlayer(player.getX(), player.getY(), map, enemies);
        } else {
            enemy.randomMove(map, enemies);
        }

        // 检测敌人是否邻接玩家
        int newDistX = std::abs(player.getX() - enemy.getX());
        int newDistY = std::abs(player.getY() - enemy.getY());
        if (newDistX + newDistY == 1) {
            render();
            showPlayerStatus();
            std::cout << std::endl;
            color(COLOR_BRIGHT_RED);
            std::cout << "Enemy approaches! Engaging combat..." << std::endl;
            resetColor();

            combat(i, true);
            return;
        }
    }
}

void Game::combat(size_t enemyIndex, bool enemyFirst) {
    Enemy& enemy = enemies[enemyIndex];

    // 使用 Renderer 渲染战斗画面
    renderer.renderCombatHeader(enemy, player, enemyFirst);

    // 委托给 Battle 类执行战斗
    bool playerWon = battle.executeCombat(player, enemy, enemyFirst, combatLog);

    std::cout << std::endl;
    renderer.drawLine(50);
    std::cout << std::endl;
    color(COLOR_YELLOW);
    std::cout << "  Press any key to continue...";
    resetColor();
    Keyboard::getKey();
}

void Game::checkGameOver() {
    if (!player.isAlive()) {
        isRunning = false;
        result = DEFEAT;
    } else if (countAliveEnemies() == 0) {
        isRunning = false;
        result = VICTORY;
    }
}

bool Game::showEndScreen() {
    renderer.clearScreen();

    std::cout << std::endl;
    renderer.drawLine(50);

    if (result == VICTORY) {
        color(COLOR_BRIGHT_GREEN);
        renderer.drawCenteredText("VICTORY!", 50);
        resetColor();
        std::cout << std::endl;
        renderer.drawCenteredText("All enemies defeated!", 50);
        renderer.drawCenteredText("You are the dungeon master!", 50);
    } else if (result == DEFEAT) {
        color(COLOR_BRIGHT_RED);
        renderer.drawCenteredText("DEFEAT", 50);
        resetColor();
        std::cout << std::endl;
        renderer.drawCenteredText("You fell in battle...", 50);
    } else {
        color(COLOR_YELLOW);
        renderer.drawCenteredText("GAME OVER", 50);
        resetColor();
    }

    std::cout << std::endl;
    renderer.drawLine(50);
    std::cout << std::endl;

    color(COLOR_BRIGHT_CYAN);
    std::cout << "  [1] Play Again (Same Mode)  [2] Return to Main Menu" << std::endl;
    resetColor();
    std::cout << std::endl;
    color(COLOR_CYAN);
    std::cout << "  Select (1/2): ";
    resetColor();

    char input = Keyboard::getKey();

    if (input == '1') {
        initGame();

        renderer.clearScreen();
        std::cout << std::endl;
        renderer.drawLine(50);
        color(COLOR_BRIGHT_GREEN);
        renderer.drawCenteredText("GAME START!", 50);
        resetColor();
        renderer.drawLine(50);
        std::cout << std::endl;
        std::cout << "  Defeat all " << settings.enemyCount << " enemies!" << std::endl;
        std::cout << std::endl;
        color(COLOR_YELLOW);
        std::cout << "  Press any key to start...";
        resetColor();
        Keyboard::getKey();
        std::cout << std::endl;

        while (isRunning) {
            render();
            showPlayerStatus();
            if (!handleInput()) {
                result = QUIT;
                break;
            }
            updateEnemies();
            checkGameOver();
        }
        return showEndScreen();
    }

    return true;
}
