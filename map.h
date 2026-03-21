#ifndef MAP_H
#define MAP_H

#include <vector>

class Map {
public:
    static const int WIDTH = 50;
    static const int HEIGHT = 50;
    static const int VIEW_RADIUS = 8;  // 增大视野半径

    Map();

    bool isWalkable(int x, int y) const;
    void render(int playerX, int playerY, const int enemies[][2], int enemyCount,
                const std::vector<std::pair<int,int>>& footprints,
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
