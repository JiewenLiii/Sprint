#include "game.h"
#include "colors.h"
#include "keyboard.h"
#include <iostream>
#include <string>
#include <cctype>

Game::Game() : isRunning(true), isVictory(false), difficulty(EASY) {
    Colors::init();
}

bool Game::showStartMenu() {
    clearScreen();
    
    std::cout << "================================" << std::endl;
    color(COLOR_BRIGHT_YELLOW);
    std::cout << "   Dungeon Adventure" << std::endl;
    resetColor();
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
    
    color(COLOR_GREEN);
    std::cout << "  [1] EASY MODE" << std::endl;
    resetColor();
    std::cout << "      - Enemies: 3" << std::endl;
    std::cout << "      - Enemy HP: 10, ATK: 3" << std::endl;
    std::cout << std::endl;
    
    color(COLOR_RED);
    std::cout << "  [2] HARD MODE" << std::endl;
    resetColor();
    std::cout << "      - Enemies: 8" << std::endl;
    std::cout << "      - Enemy HP: 15, ATK: 6" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  Select difficulty (1/2): ";
    
    char input = Keyboard::getKey();
    difficulty = (input == '2') ? HARD : EASY;
    settings = getDifficultySettings(difficulty);
    
    std::cout << std::endl;
    std::cout << "  Starting " << (difficulty == EASY ? "EASY" : "HARD") << " MODE..." << std::endl;
    std::cout << "  Press any key to start..." << std::endl;
    Keyboard::getKey();
    
    return true;
}

bool Game::showEndScreen() {
    std::cout << std::endl;
    std::cout << "  [1] Play Again  [2] Quit" << std::endl;
    std::cout << "  Select (1/2): ";
    
    char input = Keyboard::getKey();
    
    if (input == '1') {
        // 重玩时清除足迹
        footprints.clear();
        playerFootprints.clear();
        combatLog.clear();
        return true;
    }
    return false;
}

bool Game::run() {
    if (!showStartMenu()) return false;
    
    player.reset();
    footprints.clear();
    playerFootprints.clear();
    spawnEnemies(map, enemies, settings.enemyCount, 
                 settings.enemyHp, settings.enemyAttack, 
                 player.x, player.y);
    combatLog.clear();
    isRunning = true;
    isVictory = false;

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
    return showEndScreen();
}

void Game::render() {
    int enemyPositions[20][2];
    int count = 0;

    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && count < 20) {
            enemyPositions[count][0] = enemy.x;
            enemyPositions[count][1] = enemy.y;
            count++;
        }
    }

    map.render(player.x, player.y, enemyPositions, count, footprints, playerFootprints);
}

void Game::showPlayerStatus() {
    std::cout << std::endl;
    color(COLOR_BRIGHT_GREEN);
    std::cout << "Player HP: " << player.hp << "/" << player.maxHp;
    resetColor();
    std::cout << " | Attack: " << player.attack << std::endl;
    color(COLOR_RED);
    std::cout << "Enemies Remaining: " << countAliveEnemies() << std::endl;
    resetColor();
    
    if (!combatLog.empty()) {
        std::cout << std::endl;
        color(COLOR_YELLOW);
        std::cout << "[Recent Actions]" << std::endl;
        resetColor();
        size_t start = combatLog.size() > 3 ? combatLog.size() - 3 : 0;
        for (size_t i = start; i < combatLog.size(); ++i) {
            std::cout << "  " << combatLog[i] << std::endl;
        }
    }
}

int Game::countAliveEnemies() const {
    int count = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) count++;
    }
    return count;
}

bool Game::handleInput() {
    std::cout << std::endl << "Enter action (W/A/S/D/Q): ";
    
    char input = Keyboard::getKey();
    input = static_cast<char>(std::toupper(static_cast<unsigned char>(input)));

    int dx = 0, dy = 0;
    bool moved = false;

    switch (input) {
        case 'W': dy = -1; moved = player.move(dx, dy, map, enemies); break;
        case 'S': dy = 1; moved = player.move(dx, dy, map, enemies); break;
        case 'A': dx = -1; moved = player.move(dx, dy, map, enemies); break;
        case 'D': dx = 1; moved = player.move(dx, dy, map, enemies); break;
        case 'Q': isRunning = false; return false;
        default:
            std::cout << "Invalid input! Use W/A/S/D to move, Q to quit." << std::endl;
            return true;
    }

    if (moved) {
        // 添加玩家足迹
        playerFootprints.push_back({player.x - dx, player.y - dy});
        if (playerFootprints.size() > 50) {
            playerFootprints.erase(playerFootprints.begin());
        }
        
        // 先渲染移动后的画面（实时显示）
        render();
        showPlayerStatus();
        
        // 检查是否与敌人相邻，若是则触发战斗
        bool adjacentToEnemy = false;
        size_t enemyIndex = 0;
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (enemies[i].isAlive()) {
                int distX = std::abs(player.x - enemies[i].x);
                int distY = std::abs(player.y - enemies[i].y);
                if (distX <= 1 && distY <= 1 && (distX > 0 || distY > 0)) {
                    adjacentToEnemy = true;
                    enemyIndex = i;
                    break;
                }
            }
        }
        
        if (adjacentToEnemy) {
            // 等待一下让玩家看到移动结果
            std::cout << std::endl;
            color(COLOR_BRIGHT_RED);
            std::cout << "Enemy adjacent! Press any key to fight..." << std::endl;
            resetColor();
            Keyboard::getKey();
            
            // 触发战斗
            combat(enemyIndex, false);  // 玩家先手
        }
    } else {
        std::cout << "Cannot move to that position!" << std::endl;
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

        // 添加敌人足迹
        if (enemy.x != oldX || enemy.y != oldY) {
            footprints.push_back({oldX, oldY});
            if (footprints.size() > 50) {
                footprints.erase(footprints.begin());
            }
        }

        // 移动后检查是否与玩家相邻
        int newDistX = std::abs(player.x - enemy.x);
        int newDistY = std::abs(player.y - enemy.y);
        if (newDistX <= 1 && newDistY <= 1 && (newDistX > 0 || newDistY > 0)) {
            // 敌人主动攻击，先手
            combat(i, true);
            return;  // 战斗结束，不再处理其他敌人
        }
    }
}

void Game::combat(size_t enemyIndex, bool enemyFirst) {
    Enemy& enemy = enemies[enemyIndex];

    std::cout << std::endl;
    color(COLOR_BRIGHT_RED);
    std::cout << "=== Combat Started! ===" << std::endl;
    resetColor();
    std::cout << "Enemy: " << enemy.getName() << std::endl;
    std::cout << enemy.getName() << " HP: " << enemy.hp << "/" << enemy.maxHp << std::endl;
    
    if (enemyFirst) {
        color(COLOR_BRIGHT_RED);
        std::cout << "The enemy attacks first!" << std::endl;
        resetColor();
    }
    std::cout << std::endl;

    // 回合制战斗
    bool playerTurn = !enemyFirst;  // enemyFirst 为 true 时敌人先手
    
    while (enemy.isAlive() && player.isAlive()) {
        if (playerTurn) {
            // 玩家攻击
            int playerDamage = player.attack;
            enemy.takeDamage(playerDamage);
            
            color(COLOR_GREEN);
            std::cout << "You attacked " << enemy.getName() << " for " << playerDamage << " damage!" << std::endl;
            resetColor();
            showCombatLog("Hit " + enemy.getName() + " for " + std::to_string(playerDamage) + " dmg");

            if (!enemy.isAlive()) {
                color(COLOR_BRIGHT_GREEN);
                std::cout << enemy.getName() << " defeated!" << std::endl;
                resetColor();
                showCombatLog(enemy.getName() + " defeated!");
                break;
            }

            // 敌人反击
            int enemyDamage = enemy.attack;
            player.takeDamage(enemyDamage);
            
            color(COLOR_RED);
            std::cout << enemy.getName() << " attacked you for " << enemyDamage << " damage!" << std::endl;
            resetColor();
            showCombatLog(enemy.getName() + " hit you for " + std::to_string(enemyDamage) + " dmg");

            if (!player.isAlive()) {
                color(COLOR_BRIGHT_RED);
                std::cout << "You were defeated..." << std::endl;
                resetColor();
                showCombatLog("You were defeated");
                break;
            }
        } else {
            // 敌人先手攻击
            int enemyDamage = enemy.attack;
            player.takeDamage(enemyDamage);
            
            color(COLOR_RED);
            std::cout << enemy.getName() << " attacked you for " << enemyDamage << " damage!" << std::endl;
            resetColor();
            showCombatLog(enemy.getName() + " hit you for " + std::to_string(enemyDamage) + " dmg");

            if (!player.isAlive()) {
                color(COLOR_BRIGHT_RED);
                std::cout << "You were defeated..." << std::endl;
                resetColor();
                showCombatLog("You were defeated");
                break;
            }

            // 玩家反击
            int playerDamage = player.attack;
            enemy.takeDamage(playerDamage);
            
            color(COLOR_GREEN);
            std::cout << "You attacked " << enemy.getName() << " for " << playerDamage << " damage!" << std::endl;
            resetColor();
            showCombatLog("Hit " + enemy.getName() + " for " + std::to_string(playerDamage) + " dmg");

            if (!enemy.isAlive()) {
                color(COLOR_BRIGHT_GREEN);
                std::cout << enemy.getName() << " defeated!" << std::endl;
                resetColor();
                showCombatLog(enemy.getName() + " defeated!");
                break;
            }
        }

        // 显示当前血量
        std::cout << std::endl;
        std::cout << enemy.getName() << " HP: " << enemy.hp << "/" << enemy.maxHp << std::endl;
        std::cout << "Your HP: " << player.hp << "/" << player.maxHp << std::endl;

        // 切换回合
        playerTurn = !playerTurn;
    }

    std::cout << std::endl;
    std::cout << "Press any key to continue..." << std::endl;
    Keyboard::getKey();
}

void Game::showCombatLog(const std::string& message) {
    combatLog.push_back(message);
    if (combatLog.size() > 10) combatLog.erase(combatLog.begin());
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
    clearScreen();

    std::cout << "================================" << std::endl;
    if (isVictory) {
        color(COLOR_BRIGHT_GREEN);
        std::cout << "   Congratulations! You Won!" << std::endl;
        resetColor();
        std::cout << "   All enemies have been defeated!" << std::endl;
    } else {
        color(COLOR_BRIGHT_RED);
        std::cout << "   Game Over" << std::endl;
        resetColor();
        std::cout << "   You were defeated..." << std::endl;
    }
    std::cout << "================================" << std::endl;
    std::cout << "Press any key to exit..." << std::endl;
    Keyboard::getKey();  // 等待用户按键
}

void Game::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
