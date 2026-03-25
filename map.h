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
    void render(int playerX, int playerY,
                const std::vector<class Enemy>& enemies,
                const std::vector<std::vector<std::pair<int,int>>>& enemyFootprints,
                const std::vector<std::pair<int,int>>& playerFootprints) const;
    char getTile(int x, int y) const;
    bool isVisible(int playerX, int playerY, int x, int y) const;
    bool isExplored(int x, int y) const;

private:
    char grid[HEIGHT][WIDTH];
    mutable bool explored[HEIGHT][WIDTH];
    void generate();
};

#endif // MAP_H
