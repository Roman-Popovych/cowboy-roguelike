#pragma once
#include <vector>
#include <random>

class LevelGenerator {
public:
    LevelGenerator();
    std::vector<std::vector<int>> generateLevelMap();

    static const int LEVEL_SIZE = 9;
    static const int EMPTY = 0;
    static const int ROOM = 1;
    static const int TREASURE = 2;
    static const int SHOP = 3;
    static const int BOSS = 4;
    static const int MIN_ROOMS = 7;
    static const int MAX_ROOMS = 15;
    
    std::vector<std::vector<int>> getLevelMap() { return m_levelMap; }
private:
    std::vector<std::vector<int>> m_levelMap;
    std::mt19937 m_rng;
    
    void initializeLevel();
    void generateRooms();
    void placeSpecialRooms();
    bool isValidPosition(int x, int y) const;
    bool hasAdjacentRoom(int x, int y) const;
    int countAdjacentRooms(int x, int y) const;
    int countRooms() const;
}; 