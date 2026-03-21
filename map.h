#ifndef MAP_H
#define MAP_H

/**
 * @brief Map class for managing the 10x10 dungeon map
 * 
 * Map uses 2D character array:
 * - '#' represents wall (impassable)
 * - '.' represents floor (walkable)
 */
class Map {
public:
    static const int WIDTH = 10;
    static const int HEIGHT = 10;

    Map();

    bool isWalkable(int x, int y) const;
    void render(int playerX, int playerY, const int enemies[][2], int enemyCount) const;
    char getTile(int x, int y) const;

private:
    char grid[HEIGHT][WIDTH];
    void generate();
};

#endif // MAP_H
