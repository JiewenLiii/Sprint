#include "doctest.h"
#include "../ui/renderer.h"
#include "../entities/player.h"
#include "../entities/enemy.h"
#include "../world/map.h"
#include "../ui/colors.h"

// ============================================================
// 测试 4：Renderer 渲染模块
// ============================================================

TEST_CASE("Renderer Construction") {
    // Renderer 能正常构造，不抛异常
    Renderer r;
    CHECK(true);
}

TEST_CASE("Renderer DrawLine") {
    Renderer r;
    // drawLine 应正常执行不崩溃
    r.drawLine(50);
    r.drawLine(30);
    CHECK(true);
}

TEST_CASE("Renderer DrawCenteredText") {
    Renderer r;
    r.drawCenteredText("Hello", 50);
    r.drawCenteredText("", 20);
    r.drawCenteredText("A", 1);  // 宽度小于文字长度
    CHECK(true);
}

TEST_CASE("Renderer ClearScreen") {
    Renderer r;
    r.clearScreen();
    CHECK(true);
}

TEST_CASE("Renderer RenderCombatHeader") {
    Renderer r;
    Player p;
    p.reset();
    Enemy e(11, 10, 10, 3, 0);

    // 玩家先手
    r.renderCombatHeader(e, p, false);
    // 敌人先手
    r.renderCombatHeader(e, p, true);
    CHECK(true);
}

TEST_CASE("Renderer RenderPlayerStatus") {
    Renderer r;
    Player p;
    p.reset();

    std::vector<std::string> combatLog;
    combatLog.push_back("Hit Enemy 1 for 5");
    combatLog.push_back("Enemy 1 hit you for 3");

    r.renderPlayerStatus(p, 3, 4, combatLog);
    CHECK(true);
}

TEST_CASE("Renderer RenderPlayerStatus With Enemy List") {
    Renderer r;
    Player p;
    p.reset();

    std::vector<Enemy> enemies;
    enemies.emplace_back(5, 5, 10, 3, 0);
    enemies.emplace_back(8, 8, 10, 3, 1);
    enemies.emplace_back(3, 3, 10, 3, 2);
    enemies[2].takeDamage(999);  // 第 3 个敌人已死

    std::vector<std::string> combatLog;
    r.renderPlayerStatus(p, 2, 3, combatLog, &enemies);
    CHECK(true);
}

TEST_CASE("Renderer RenderGame") {
    Renderer r;
    Player p;
    p.reset();
    Map map;
    std::vector<Enemy> enemies;

    r.renderGame(map, p, enemies);
    CHECK(true);
}

TEST_CASE("Colors Init") {
    // Colors::init() 应正常执行不崩溃
    Colors::init();
    CHECK(true);
}

TEST_CASE("Colors Set And Reset") {
    Colors::init();
    Colors::set(COLOR_RED);
    Colors::set(COLOR_GREEN);
    Colors::set(COLOR_BRIGHT_CYAN);
    Colors::reset();
    CHECK(true);
}

TEST_CASE("Color Inline Functions") {
    // color() 和 resetColor() 应正常执行不崩溃
    color(COLOR_RED);
    resetColor();
    color(COLOR_BRIGHT_GREEN);
    resetColor();
    CHECK(true);
}

TEST_CASE("ColorScope RAII") {
    {
        ColorScope scope(COLOR_YELLOW);
        // 在此作用域内颜色应被设置
    }
    // 离开作用域后颜色应被重置
    CHECK(true);
}
