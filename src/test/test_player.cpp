#include "doctest.h"
#include "../entities/entity.h"
#include "../entities/player.h"
#include "../world/map.h"
#include "../entities/enemy.h"
#include "../ui/colors.h"

// 测试用具体子类（Entity 是抽象类，有纯虚函数，不能直接实例化）
class TestEntity : public Entity {
public:
    TestEntity(int x, int y, int hp, int attack)
        : Entity(x, y, hp, attack) {}
    char getSymbol() const override { return 'T'; }
    std::string getName() const override { return "TestEntity"; }
};

// ============================================================
// 测试 1：Entity 实体基类
// ============================================================

TEST_CASE("Entity Construction") {
    TestEntity e(5, 10, 100, 20);
    CHECK(e.getX() == 5);
    CHECK(e.getY() == 10);
    CHECK(e.getHp() == 100);
    CHECK(e.getMaxHp() == 100);
    CHECK(e.getAttack() == 20);
    CHECK(e.isAlive() == true);
}

TEST_CASE("Entity TakeDamage") {
    TestEntity e(0, 0, 100, 10);
    e.takeDamage(30);
    CHECK(e.getHp() == 70);
    CHECK(e.isAlive() == true);
}

TEST_CASE("Entity TakeDamage Lethal") {
    TestEntity e(0, 0, 100, 10);
    e.takeDamage(200);
    CHECK(e.getHp() == 0);
    CHECK(e.isAlive() == false);
}

TEST_CASE("Entity TakeDamage Exact Kill") {
    TestEntity e(0, 0, 50, 10);
    e.takeDamage(50);
    CHECK(e.getHp() == 0);
    CHECK(e.isAlive() == false);
}

TEST_CASE("Entity SetHp Clamp Upper") {
    TestEntity e(0, 0, 100, 10);
    e.setHp(999);
    CHECK(e.getHp() == 100);  // 不超过 maxHp
}

TEST_CASE("Entity SetHp Clamp Lower") {
    TestEntity e(0, 0, 100, 10);
    e.setHp(-10);
    CHECK(e.getHp() == 0);  // 不低于 0
}

TEST_CASE("Entity SetPosition") {
    TestEntity e(0, 0, 100, 10);
    e.setPosition(7, 13);
    CHECK(e.getX() == 7);
    CHECK(e.getY() == 13);
}

TEST_CASE("Entity SetAttack") {
    TestEntity e(0, 0, 100, 10);
    e.setAttack(25);
    CHECK(e.getAttack() == 25);
}

TEST_CASE("Entity GetHealthBar") {
    TestEntity e(0, 0, 100, 10);
    std::string bar = e.getHealthBar();
    CHECK(bar == "HP: 100/100");
}

// ============================================================
// 测试 2：Player 玩家模块
// ============================================================

TEST_CASE("Player Reset") {
    Player p;
    p.reset();
    CHECK(p.getX() == Map::WIDTH / 2);   // 10
    CHECK(p.getY() == Map::HEIGHT / 2);  // 10
    CHECK(p.getHp() == 20);
    CHECK(p.getMaxHp() == 20);
    CHECK(p.getAttack() == 5);
}

TEST_CASE("Player Move Valid") {
    Player p;
    p.reset();
    Map map;
    std::vector<Enemy> enemies;

    bool moved = p.move(1, 0, map, enemies);  // 向右移动
    CHECK(moved == true);
    CHECK(p.getX() == 11);
    CHECK(p.getY() == 10);
}

TEST_CASE("Player Move Into Wall") {
    Player p;
    p.reset();
    Map map;
    std::vector<Enemy> enemies;

    p.setPosition(1, 10);
    bool moved = p.move(-1, 0, map, enemies);  // 向左撞墙
    CHECK(moved == false);
    CHECK(p.getX() == 1);  // 位置不变
}

TEST_CASE("Player Move Out Of Bounds") {
    Player p;
    p.reset();
    Map map;
    std::vector<Enemy> enemies;

    bool moved = p.move(0, -20, map, enemies);  // 大幅向上超出边界
    CHECK(moved == false);
}

TEST_CASE("Player Move Into Enemy") {
    Player p;
    p.reset();
    Map map;

    Enemy e(11, 10, 10, 3, 0);
    std::vector<Enemy> enemies;
    enemies.push_back(e);

    bool moved = p.move(1, 0, map, enemies);  // 向右移动到敌人位置
    CHECK(moved == false);
    CHECK(p.getX() == 10);  // 位置不变
}

TEST_CASE("Player Move Past Dead Enemy") {
    Player p;
    p.reset();
    Map map;

    Enemy e(11, 10, 10, 3, 0);
    e.takeDamage(999);  // 击杀
    std::vector<Enemy> enemies;
    enemies.push_back(e);

    bool moved = p.move(1, 0, map, enemies);
    CHECK(moved == true);  // 可以穿过死亡敌人
    CHECK(p.getX() == 11);
}

TEST_CASE("Player Footprints Max 5") {
    Player p;
    p.reset();
    Map map;
    std::vector<Enemy> enemies;

    // 连续移动 7 次，足迹应最多保留 5 个
    for (int i = 0; i < 7; ++i) {
        p.move(1, 0, map, enemies);
    }

    const auto& fp = p.getFootprints();
    CHECK(fp.size() == 5);
    CHECK(fp[0].first == 12);  // 第 3 次移动前的位置
}

TEST_CASE("Player Footprints Cleared On Reset") {
    Player p;
    p.reset();
    Map map;
    std::vector<Enemy> enemies;

    p.move(1, 0, map, enemies);
    p.move(1, 0, map, enemies);
    CHECK(p.getFootprints().size() == 2);

    p.reset();
    CHECK(p.getFootprints().size() == 0);
}

TEST_CASE("Player Status String") {
    Player p;
    p.reset();
    std::string status = p.getStatus();
    CHECK(status.find("HP: 20/20") != std::string::npos);
    CHECK(status.find("ATK: 5") != std::string::npos);
}

TEST_CASE("Player Symbol And Name") {
    Player p;
    CHECK(p.getSymbol() == '@');
    CHECK(p.getName() == "Player");
}
