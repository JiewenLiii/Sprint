#ifndef ENTITY_H
#define ENTITY_H

#include <string>

/**
 * @brief 实体基类 - 玩家和敌人的共同父类
 * 
 * 封装公共属性：坐标、HP、攻击力
 * 提供公共方法：移动、受伤、死亡判断
 */
class Entity {
public:
    Entity(int x, int y, int hp, int attack);
    virtual ~Entity() = default;

    // 坐标访问器
    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int x, int y);

    // HP 访问器
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    void setHp(int hp);

    // 攻击力访问器
    int getAttack() const { return attack; }
    void setAttack(int attack);

    // 公共方法
    virtual void takeDamage(int damage);
    bool isAlive() const;
    virtual std::string getHealthBar() const;

    // 纯虚函数 - 子类必须实现
    virtual char getSymbol() const = 0;
    virtual std::string getName() const = 0;

protected:
    int x, y;           // 坐标
    int hp;             // 当前生命值
    int maxHp;          // 最大生命值
    int attack;         // 攻击力
};

#endif // ENTITY_H
