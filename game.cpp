#include "game.h"
#include "colors.h"
#include "keyboard.h"
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

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
    clearScreen();

    std::cout << std::endl;
    drawLine(60);
    color(COLOR_BRIGHT_YELLOW);
    drawCenteredText("⚔  DUNGEON ADVENTURE  ⚔", 60);
    resetColor();
    color(COLOR_WHITE);
    drawCenteredText("Explore the dungeon, defeat all enemies!", 60);
    resetColor();
    std::cout << std::endl;
    drawLine(60);
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
    
    drawLine(60);
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
            drawLine(60);
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

    clearScreen();
    std::cout << std::endl;
    drawLine(50);
    color(COLOR_BRIGHT_GREEN);
    drawCenteredText("GAME START!", 50);
    resetColor();
    drawLine(50);
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

void Game::initGame() {
    player.reset();
    spawnEnemies(map, enemies, settings.enemyCount,
                 settings.enemyHp, settings.enemyAttack,
                 player.x, player.y);
    enemyFootprints.clear();
    enemyFootprints.resize(enemies.size());  // 为每个敌人创建足迹向量
    playerFootprints.clear();
    combatLog.clear();
    isRunning = true;
    result = QUIT;
}

void Game::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Game::drawLine(int width) {
    color(COLOR_CYAN);
    for (int i = 0; i < width; ++i) std::cout << "=";
    resetColor();
    std::cout << std::endl;
}

void Game::drawCenteredText(const std::string& text, int width) {
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding < 0) padding = 0;
    for (int i = 0; i < padding; ++i) std::cout << " ";
    std::cout << text << std::endl;
}

void Game::render() {
    clearScreen();

    drawLine(50);
    color(COLOR_BRIGHT_YELLOW);
    drawCenteredText("DUNGEON ADVENTURE", 50);
    resetColor();
    drawLine(50);

    map.render(player.x, player.y, enemies, enemyFootprints, playerFootprints);
}

void Game::showPlayerStatus() {
    std::cout << std::endl;
    drawLine(50);

    color(COLOR_BRIGHT_GREEN);
    std::cout << "  " << player.getStatus() << std::endl;
    resetColor();

    color(COLOR_RED);
    std::cout << "  Enemies: " << countAliveEnemies() << " / " << settings.enemyCount << std::endl;
    resetColor();

    std::cout << "  Remaining: ";
    bool first = true;
    for (const auto& e : enemies) {
        if (e.isAlive()) {
            if (!first) std::cout << ", ";
            int colorIdx = e.getId() % 4;
            switch (colorIdx) {
                case 0: color(COLOR_RED); break;
                case 1: color(COLOR_BRIGHT_RED); break;
                case 2: color(COLOR_MAGENTA); break;
                case 3: color(COLOR_BRIGHT_MAGENTA); break;
            }
            std::cout << e.getSymbol() << "(" << e.getName() << ")";
            resetColor();
            first = false;
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;

    if (!combatLog.empty()) {
        std::cout << std::endl;
        color(COLOR_YELLOW);
        std::cout << "  [Recent]" << std::endl;
        resetColor();
        size_t start = combatLog.size() > 4 ? combatLog.size() - 4 : 0;
        for (size_t i = start; i < combatLog.size(); ++i) {
            std::cout << "    " << combatLog[i] << std::endl;
        }
    }
    std::cout << std::endl;
}

int Game::countAliveEnemies() const {
    int count = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) count++;
    }
    return count;
}

int Game::calculateDamage(int attackerAtk, int defenderDef) const {
    int damage = attackerAtk - defenderDef;
    return std::max(1, damage);
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
        playerFootprints.push_back({player.x - dx, player.y - dy});
        if (playerFootprints.size() > 5) {
            playerFootprints.erase(playerFootprints.begin());
        }

        bool adjacentToEnemy = false;
        size_t enemyIndex = 0;
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (enemies[i].isAlive()) {
                int distX = std::abs(player.x - enemies[i].x);
                int distY = std::abs(player.y - enemies[i].y);
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

        int oldX = enemy.x;
        int oldY = enemy.y;
        int distX = std::abs(player.x - enemy.x);
        int distY = std::abs(player.y - enemy.y);

        if (distX <= CHASE_RADIUS && distY <= CHASE_RADIUS) {
            enemy.moveTowardsPlayer(player.x, player.y, map, enemies);
        } else {
            enemy.randomMove(map, enemies);
        }

        if (enemy.x != oldX || enemy.y != oldY) {
            enemyFootprints[i].push_back({oldX, oldY});
            if (enemyFootprints[i].size() > 5) {
                enemyFootprints[i].erase(enemyFootprints[i].begin());
            }
        }

        int newDistX = std::abs(player.x - enemy.x);
        int newDistY = std::abs(player.y - enemy.y);
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

    std::cout << std::endl;
    drawLine(50);
    color(COLOR_BRIGHT_RED);
    drawCenteredText("COMBAT!", 50);
    resetColor();
    drawLine(50);
    std::cout << std::endl;

    color(COLOR_YELLOW);
    std::cout << "  Enemy: " << enemy.getName() << std::endl;
    resetColor();
    std::cout << "  " << enemy.getHealthBar() << std::endl;

    color(COLOR_RED);
    std::cout << "  Enemy ATK: " << enemy.attack << std::endl;
    resetColor();
    std::cout << std::endl;

    if (enemyFirst) {
        color(COLOR_RED);
        std::cout << "  Enemy attacks first!" << std::endl;
        resetColor();
    } else {
        color(COLOR_GREEN);
        std::cout << "  You attack first!" << std::endl;
        resetColor();
    }
    std::cout << std::endl;

    // 回合制战斗 - 固定顺序，不交换
    while (enemy.isAlive() && player.isAlive()) {
        if (enemyFirst) {
            // 敌人先手：敌人攻击 → 玩家反击
            int enemyDamage = enemy.attack;
            player.takeDamage(enemyDamage);

            color(COLOR_RED);
            std::cout << "  " << enemy.getName() << " attacks you for " << enemyDamage << " damage!" << std::endl;
            resetColor();
            addCombatLog(enemy.getName() + " hit you for " + std::to_string(enemyDamage));

            if (!player.isAlive()) {
                color(COLOR_RED);
                std::cout << "  You were defeated..." << std::endl;
                resetColor();
                addCombatLog("You were defeated");
                break;
            }

            int playerDamage = player.attack;
            enemy.takeDamage(playerDamage);

            color(COLOR_GREEN);
            std::cout << "  You attack " << enemy.getName() << " for " << playerDamage << " damage!" << std::endl;
            resetColor();
            addCombatLog("Hit " + enemy.getName() + " for " + std::to_string(playerDamage));

            if (!enemy.isAlive()) {
                color(COLOR_BRIGHT_GREEN);
                std::cout << "  " << enemy.getName() << " defeated!" << std::endl;
                resetColor();
                addCombatLog(enemy.getName() + " defeated");
                break;
            }
        } else {
            // 玩家先手：玩家攻击 → 敌人反击
            int playerDamage = player.attack;
            enemy.takeDamage(playerDamage);

            color(COLOR_GREEN);
            std::cout << "  You attack " << enemy.getName() << " for " << playerDamage << " damage!" << std::endl;
            resetColor();
            addCombatLog("Hit " + enemy.getName() + " for " + std::to_string(playerDamage));

            if (!enemy.isAlive()) {
                color(COLOR_BRIGHT_GREEN);
                std::cout << "  " << enemy.getName() << " defeated!" << std::endl;
                resetColor();
                addCombatLog(enemy.getName() + " defeated");
                break;
            }

            int enemyDamage = enemy.attack;
            player.takeDamage(enemyDamage);

            color(COLOR_RED);
            std::cout << "  " << enemy.getName() << " attacks you for " << enemyDamage << " damage!" << std::endl;
            resetColor();
            addCombatLog(enemy.getName() + " hit you for " + std::to_string(enemyDamage));

            if (!player.isAlive()) {
                color(COLOR_RED);
                std::cout << "  You were defeated..." << std::endl;
                resetColor();
                addCombatLog("You were defeated");
                break;
            }
        }

        std::cout << std::endl;
        std::cout << "  " << enemy.getName() << ": " << enemy.getHealthBar() << std::endl;
        std::cout << "  You: " << player.getHealthBar() << std::endl;
        std::cout << std::endl;
    }

    std::cout << std::endl;
    drawLine(50);
    std::cout << std::endl;
    color(COLOR_YELLOW);
    std::cout << "  Press any key to continue...";
    resetColor();
    Keyboard::getKey();
}

void Game::addCombatLog(const std::string& message) {
    combatLog.push_back(message);
    if (combatLog.size() > 10) combatLog.erase(combatLog.begin());
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
    clearScreen();

    std::cout << std::endl;
    drawLine(50);

    if (result == VICTORY) {
        color(COLOR_BRIGHT_GREEN);
        drawCenteredText("VICTORY!", 50);
        resetColor();
        std::cout << std::endl;
        drawCenteredText("All enemies defeated!", 50);
        drawCenteredText("You are the dungeon master!", 50);
    } else if (result == DEFEAT) {
        color(COLOR_BRIGHT_RED);
        drawCenteredText("DEFEAT", 50);
        resetColor();
        std::cout << std::endl;
        drawCenteredText("You fell in battle...", 50);
    } else {
        color(COLOR_YELLOW);
        drawCenteredText("GAME OVER", 50);
        resetColor();
    }

    std::cout << std::endl;
    drawLine(50);
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

        clearScreen();
        std::cout << std::endl;
        drawLine(50);
        color(COLOR_BRIGHT_GREEN);
        drawCenteredText("GAME START!", 50);
        resetColor();
        drawLine(50);
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
