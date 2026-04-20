#include "doctest.h"
#include "../core/battle.h"
#include "../entities/player.h"
#include "../entities/enemy.h"
#include "../world/map.h"
#include "../ui/colors.h"

// ============================================================
// 测试 3：Battle 战斗模块 + Enemy 敌人模块
// ============================================================

// --- 伤害计算 ---

TEST_CASE("Damage Basic") {
    Battle b;
    CHECK(b.calculateDamage(10, 3) == 7);
}

TEST_CASE("Damage Zero Defense") {
    Battle b;
    CHECK(b.calculateDamage(5, 0) == 5);
}

TEST_CASE("Damage Minimum 1") {
    Battle b;
    CHECK(b.calculateDamage(1, 10) == 1);  // 伤害最低为 1
}

TEST_CASE("Damage Equal AtkDef") {
    Battle b;
    CHECK(b.calculateDamage(5, 5) == 1);  // max(1, 0) = 1
}

TEST_CASE("Damage Negative Defense") {
    Battle b;
    CHECK(b.calculateDamage(10, -5) == 15);
}

// --- 战斗日志 ---

TEST_CASE("CombatLog Add") {
    Battle b;
    std::vector<std::string> log;
    b.addCombatLog("msg1", log);
    b.addCombatLog("msg2", log);
    b.addCombatLog("msg3", log);
    CHECK(log.size() == 3);
    CHECK(log[0] == "msg1");
    CHECK(log[2] == "msg3");
}

TEST_CASE("CombatLog Overflow Max 10") {
    Battle b;
    std::vector<std::string> log;
    for (int i = 0; i < 12; ++i) {
        b.addCombatLog("log" + std::to_string(i), log);
    }
    CHECK(log.size() == 10);
    CHECK(log[0] == "log2");   // 最早的 2 条被淘汰
    CHECK(log[9] == "log11");
}

// --- 战斗结果判定 ---

TEST_CASE("Combat Player Wins One Shot") {
    Player p;
    p.reset();  // HP=20, ATK=5
    Enemy e(11, 10, 5, 1, 0);  // HP=5, ATK=1

    Battle b;
    std::vector<std::string> log;
    bool result = b.executeCombat(p, e, false, log);  // 玩家先手

    CHECK(result == true);
    CHECK(e.isAlive() == false);
    CHECK(p.isAlive() == true);
}

TEST_CASE("Combat Player Wins Multiple Rounds") {
    Player p;
    p.reset();  // HP=20, ATK=5
    Enemy e(11, 10, 10, 1, 0);  // HP=10, ATK=1

    Battle b;
    std::vector<std::string> log;
    bool result = b.executeCombat(p, e, false, log);

    CHECK(result == true);
    CHECK(e.isAlive() == false);
    CHECK(p.isAlive() == true);
}

TEST_CASE("Combat Enemy Wins") {
    Player p;
    p.reset();
    p.setHp(5);
    p.setAttack(1);

    Enemy e(11, 10, 50, 10, 0);

    Battle b;
    std::vector<std::string> log;
    bool result = b.executeCombat(p, e, true, log);  // 敌人先手

    CHECK(result == false);
    CHECK(p.isAlive() == false);
    CHECK(e.isAlive() == true);
}

TEST_CASE("Combat Enemy First Strike Kill") {
    Player p;
    p.reset();
    p.setHp(1);
    p.setAttack(1);

    Enemy e(11, 10, 50, 10, 0);

    Battle b;
    std::vector<std::string> log;
    bool result = b.executeCombat(p, e, true, log);

    CHECK(result == false);
    CHECK(p.isAlive() == false);
}

// --- 难度设置 ---

TEST_CASE("Difficulty Settings Easy") {
    DifficultySettings s = getDifficultySettings(EASY);
    CHECK(s.enemyCount == 4);
    CHECK(s.enemyHp == 10);
    CHECK(s.enemyAttack == 3);
}

TEST_CASE("Difficulty Settings Hard") {
    DifficultySettings s = getDifficultySettings(HARD);
    CHECK(s.enemyCount == 8);
    CHECK(s.enemyHp == 15);
    CHECK(s.enemyAttack == 6);
}

// --- 敌人构造 ---

TEST_CASE("Enemy Construction") {
    Enemy e(5, 5, 15, 6, 3);
    CHECK(e.getX() == 5);
    CHECK(e.getY() == 5);
    CHECK(e.getHp() == 15);
    CHECK(e.getMaxHp() == 15);
    CHECK(e.getAttack() == 6);
    CHECK(e.getId() == 3);
    CHECK(e.isAlive() == true);
}

TEST_CASE("Enemy Symbol And Name") {
    Enemy e(0, 0, 10, 3, 2);
    CHECK(e.getSymbol() == 'E');
    CHECK(e.getName() == "Enemy 3");  // id + 1
}

TEST_CASE("Enemy Default Construction") {
    Enemy e;
    CHECK(e.getHp() == 10);
    CHECK(e.getAttack() == 3);
    CHECK(e.getId() == 0);
}
