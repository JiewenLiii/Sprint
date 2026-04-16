#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Map {
public:
    static const int WIDTH = 20;
    static const int HEIGHT = 20;
    static const int VIEW_RADIUS = 6;  // 圆形视野半径

    Map();

    bool isWalkable(int x, int y) const;
    char getTile(int x, int y) const;
    bool isVisible(int playerX, int playerY, int x, int y) const;
    bool isExplored(int x, int y) const;
    
    // 更新已探索区域（由渲染系统调用）
    void updateExplored(int playerX, int playerY) const;

private:
    char grid[HEIGHT][WIDTH];
    mutable bool explored[HEIGHT][WIDTH];  // mutable 允许 const 方法修改
    void generate();
};

#endif // MAP_H
