#include "renderer.h"
#include "colors.h"
#include "keyboard.h"
#include <iostream>
#include <algorithm>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

Renderer::Renderer() {}

void Renderer::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Renderer::drawLine(int width) const {
    color(COLOR_CYAN);
    for (int i = 0; i < width; ++i) std::cout << "=";
    resetColor();
    std::cout << std::endl;
}

void Renderer::drawCenteredText(const std::string& text, int width) const {
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding < 0) padding = 0;
    for (int i = 0; i < padding; ++i) std::cout << " ";
    std::cout << text << std::endl;
}

void Renderer::renderEntity(char symbol, int colorIdx) const {
    switch (colorIdx % 4) {
        case 0: color(COLOR_RED); break;
        case 1: color(COLOR_BRIGHT_RED); break;
        case 2: color(COLOR_MAGENTA); break;
        case 3: color(COLOR_BRIGHT_MAGENTA); break;
    }
    std::cout << symbol;
    resetColor();
}

bool Renderer::renderFootprint(int x, int y, 
                                const std::vector<std::pair<int, int>>& footprints, 
                                bool isPlayer) const {
    for (size_t i = 0; i < footprints.size(); ++i) {
        const auto& fp = footprints[i];
        if (fp.first == x && fp.second == y) {
            size_t age = footprints.size() - i;  // 1=最新，5=最老
            if (isPlayer) {
                // 玩家足迹 - 绿色渐变
                if (age <= 2) {
                    color(COLOR_BRIGHT_GREEN);  // 最近的 2 个：亮绿色
                } else {
                    color(COLOR_GREEN);  // 其余：普通绿色
                }
            } else {
                // 敌人足迹 - 黄色渐变
                if (age <= 2) {
                    color(COLOR_BRIGHT_YELLOW);  // 最近的 2 个：亮黄色
                } else {
                    color(COLOR_YELLOW);  // 其余：普通黄色
                }
            }
            std::cout << (isPlayer ? '*' : ',');
            resetColor();
            return true;
        }
    }
    return false;
}

bool Renderer::renderEnemyFootprint(int x, int y,
                                     const std::vector<std::vector<std::pair<int, int>>>& enemyFootprints) const {
    for (size_t e = 0; e < enemyFootprints.size(); ++e) {
        const auto& eFootprints = enemyFootprints[e];
        for (size_t i = 0; i < eFootprints.size(); ++i) {
            const auto& fp = eFootprints[i];
            if (fp.first == x && fp.second == y) {
                size_t age = eFootprints.size() - i;  // 1=最新，5=最老
                // 渐变：亮黄 -> 黄
                if (age <= 2) {
                    color(COLOR_BRIGHT_YELLOW);  // 最近的 2 个：亮黄色
                } else {
                    color(COLOR_YELLOW);  // 其余：普通黄色
                }
                std::cout << ',';
                resetColor();
                return true;
            }
        }
    }
    return false;
}

void Renderer::renderMap(const Map& map, const Player& player, 
                         const std::vector<Enemy>& enemies) const {
    // 更新已探索区域
    map.updateExplored(player.getX(), player.getY());
    
    // Top border
    std::cout << "  ";
    color(COLOR_BRIGHT_CYAN);
    std::cout << "+";
    for (int x = 0; x < Map::WIDTH; ++x) std::cout << "-";
    std::cout << "+";
    resetColor();
    std::cout << std::endl;

    for (int y = 0; y < Map::HEIGHT; ++y) {
        std::cout << "  ";
        color(COLOR_BRIGHT_CYAN);
        std::cout << "|";
        resetColor();

        for (int x = 0; x < Map::WIDTH; ++x) {
            bool visible = map.isVisible(player.getX(), player.getY(), x, y);
            bool isExploredTile = map.isExplored(x, y);

            if (!isExploredTile) {
                color(COLOR_BLACK);
                std::cout << "?";
                resetColor();
                continue;
            }

            // 玩家位置
            if (x == player.getX() && y == player.getY()) {
                color(COLOR_BRIGHT_GREEN);
                std::cout << '@';
                resetColor();
                continue;
            }

            // 敌人位置（仅可见时）
            if (visible) {
                bool isEnemy = false;
                for (const auto& enemy : enemies) {
                    if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
                        renderEntity(enemy.getSymbol(), enemy.getId());
                        isEnemy = true;
                        break;
                    }
                }
                if (isEnemy) continue;
            }

            // 足迹渲染
            if (visible) {
                // 敌人足迹
                std::vector<std::vector<std::pair<int, int>>> enemyFootprints;
                for (const auto& enemy : enemies) {
                    if (enemy.isAlive()) {
                        enemyFootprints.push_back(enemy.getFootprints());
                    } else {
                        enemyFootprints.push_back(std::vector<std::pair<int, int>>());
                    }
                }
                
                if (renderEnemyFootprint(x, y, enemyFootprints)) continue;

                // 玩家足迹
                if (renderFootprint(x, y, player.getFootprints(), true)) continue;
            }

            // 地形渲染
            if (visible) {
                if (map.getTile(x, y) == '#') {
                    color(COLOR_BRIGHT_BLUE);
                    std::cout << '#';
                    resetColor();
                } else {
                    std::cout << '.';
                }
            } else {
                if (map.getTile(x, y) == '.') {
                    color(COLOR_BLACK);
                    std::cout << ':';
                    resetColor();
                } else {
                    color(COLOR_BLUE);
                    std::cout << '#';
                    resetColor();
                }
            }
        }

        color(COLOR_BRIGHT_CYAN);
        std::cout << "|";
        resetColor();
        std::cout << std::endl;
    }

    // Bottom border
    std::cout << "  ";
    color(COLOR_BRIGHT_CYAN);
    std::cout << "+";
    for (int x = 0; x < Map::WIDTH; ++x) std::cout << "-";
    std::cout << "+";
    resetColor();
    std::cout << std::endl;

    std::cout << "  Position: (" << player.getX() << ", " << player.getY() << ")";
}

void Renderer::renderGame(const Map& map, const Player& player, 
                          const std::vector<Enemy>& enemies) const {
    clearScreen();

    drawLine(50);
    color(COLOR_BRIGHT_YELLOW);
    drawCenteredText("DUNGEON ADVENTURE", 50);
    resetColor();
    drawLine(50);

    renderMap(map, player, enemies);
}

void Renderer::renderPlayerStatus(const Player& player, int aliveEnemies, int totalEnemies,
                                   const std::vector<std::string>& combatLog,
                                   const std::vector<Enemy>* enemies) const {
    std::cout << std::endl;
    drawLine(50);

    color(COLOR_BRIGHT_GREEN);
    std::cout << "  " << player.getStatus() << std::endl;
    resetColor();

    color(COLOR_RED);
    std::cout << "  Enemies: " << aliveEnemies << " / " << totalEnemies << std::endl;
    resetColor();

    // 显示存活敌人列表
    if (enemies != nullptr) {
        std::cout << "  Remaining: ";
        bool first = true;
        for (const auto& e : *enemies) {
            if (e.isAlive()) {
                if (!first) std::cout << ", ";
                switch (e.getId() % 4) {
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
    }
    std::cout << std::endl;

    // 显示战斗日志
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

void Renderer::renderCombatHeader(const Enemy& enemy, const Player& player, bool enemyFirst) const {
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
    std::cout << "  Enemy ATK: " << enemy.getAttack() << std::endl;
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
}
