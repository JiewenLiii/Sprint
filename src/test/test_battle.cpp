#include "doctest.h"
#include "../core/battle.h"
#include "../entities/player.h"
#include "../entities/enemy.h"
#include "../world/map.h"
#include "../ui/colors.h"

// ============================================================
// 测试 3：Battle 战斗模块 + Enemy 敌人模块
// ============================================================


//原代码
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


//AI 代码
//---伤害计算---
TEST_CASE("Battle Damage Calculation Full Coverage") {
    Battle battle;
    // ① 正常伤害场景（atk > def）
    CHECK(battle.calculateDamage(10, 5) == 5);
    // ② 防御值为0（全额伤害）
    CHECK(battle.calculateDamage(10, 0) == 10);
    // ③ 攻击力 < 防御力（触发最低伤害1）
    CHECK(battle.calculateDamage(5, 10) == 1);
    // ④ 攻击力 = 防御力（触发最低伤害1）
    CHECK(battle.calculateDamage(8, 8) == 1);
    // ⑤ 防御值为负数（易伤增伤场景）
    CHECK(battle.calculateDamage(10, -3) == 13);
    CHECK(battle.calculateDamage(1, -5) == 6);
}

//---战斗执行---
// ① 玩家先手，直接秒杀敌人
TEST_CASE("Combat Player Wins One Shot") {
    Player p; p.reset();
    Enemy e(50, 0, 10, 1, 0); // 高攻玩家秒杀低血敌人
    Battle b;
    std::vector<std::string> log;
    bool result = b.executeCombat(p, e, false, log);

    // 核心验证
    CHECK(result == true);
    CHECK(p.isAlive() == true);
    CHECK(e.isAlive() == false);
    CHECK(p.getHp() > 0);
    CHECK(e.getHp() == 0);
    CHECK(log.empty() == false);
    CHECK(log.size() <= 10);
}

// ② 敌人先手，直接秒杀玩家
TEST_CASE("Combat Enemy Wins Instakill") {
    Player p; p.reset();
    Enemy e(99, 0, 100, 1, 0); // 敌人超高攻击秒杀玩家
    Battle b;
    std::vector<std::string> log;
    bool result = b.executeCombat(p, e, true, log);

    // 核心验证
    CHECK(result == false);
    CHECK(p.isAlive() == false);
    CHECK(e.isAlive() == true);
    CHECK(p.getHp() == 0);
    CHECK(e.getHp() > 0);
    CHECK(log.empty() == false);
    CHECK(log.size() <= 10);
}

// ③ 多回合战斗，玩家最终胜利
TEST_CASE("Combat Multi Round Player Win") {
    Player p; p.reset();
    Enemy e(15, 5, 80, 1, 0); // 均衡属性，触发多回合战斗
    Battle b;
    std::vector<std::string> log;
    bool result = b.executeCombat(p, e, false, log);

    // 核心验证
    CHECK(result == true);
    CHECK(p.isAlive() == true);
    CHECK(e.isAlive() == false);
    CHECK(log.size() >= 3); // 多回合日志数量达标
    CHECK(log.size() <= 10); // 遵守日志最大10条规则
}

// ④ 战斗日志数量与规则校验
TEST_CASE("Combat Log Validation") {
    Player p; p.reset();
    Enemy e(20, 5, 50, 1, 0);
    Battle b;
    std::vector<std::string> log;
    b.executeCombat(p, e, false, log);

    // 验证日志核心规则
    CHECK(log.empty() == false);
    CHECK(log.size() >= 1);
    CHECK(log.size() <= 10); // 强制验证日志上限规则
}
//---实体受伤---
// 普通受伤：伤害小于血量，正常扣减，角色存活
TEST_CASE("Entity TakeDamage Normal") { Player p; p.reset(); p.takeDamage(5); CHECK(p.getHp() == 15); CHECK(p.isAlive() == true); }

// 精准致命：伤害等于血量，血量归零
TEST_CASE("Entity TakeDamage Exact Kill") { Player p; p.reset(); p.takeDamage(20); CHECK(p.getHp() == 0); }

// 过量伤害：伤害大于血量，血量锁0，无负数
TEST_CASE("Entity TakeDamage Overflow") { Player p; p.reset(); p.takeDamage(50); CHECK(p.getHp() == 0); }

// 死亡状态校验：血量归零后，存活状态为false
TEST_CASE("Entity TakeDamage Death State") { Player p; p.reset(); p.takeDamage(30); CHECK(p.isAlive() == false); }

//---战斗日志---
// 正常添加日志（数量＜10）
TEST_CASE("CombatLog Add Normal") { Battle b; std::vector<std::string> log; b.addCombatLog("test",log); b.addCombatLog("test",log); CHECK(log.size() == 2); }

// 日志溢出触发淘汰规则
TEST_CASE("CombatLog Overflow") { Battle b; std::vector<std::string> log; for(int i=0;i<12;i++) b.addCombatLog("test",log); CHECK(log.size()==10); }

// 日志数量上限恒定为10条
TEST_CASE("CombatLog Max Size Limit") { Battle b; std::vector<std::string> log; for(int i=0;i<20;i++) b.addCombatLog("test",log); CHECK(log.size() == 10); }
