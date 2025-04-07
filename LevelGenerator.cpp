#include "LevelGenerator.h"

LevelGenerator::LevelGenerator() : m_rng(std::random_device()()) {
    initializeLevel();
}

void LevelGenerator::initializeLevel() {
    m_levelMap.resize(LEVEL_SIZE, std::vector<int>(LEVEL_SIZE, EMPTY));
    m_levelMap[LEVEL_SIZE / 2][LEVEL_SIZE / 2] = ROOM;
}

bool LevelGenerator::isValidPosition(int x, int y) const {
    return x >= 0 && x < LEVEL_SIZE && y >= 0 && y < LEVEL_SIZE;
}

bool LevelGenerator::hasAdjacentRoom(int x, int y) const {
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        
        if (isValidPosition(newX, newY) && m_levelMap[newX][newY] == ROOM) {
            return true;
        }
    }
    
    return false;
}

int LevelGenerator::countAdjacentRooms(int x, int y) const {
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    int count = 0;
    
    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        
        if (isValidPosition(newX, newY) && m_levelMap[newX][newY] == ROOM) {
            count++;
        }
    }
    
    return count;
}

int LevelGenerator::countRooms() const {
    int count = 0;
    for (const auto& row : m_levelMap) {
        for (int cell : row) {
            if (cell == ROOM) count++;
        }
    }
    return count;
}

void LevelGenerator::placeSpecialRooms() {
    std::uniform_int_distribution<int> dist(0, LEVEL_SIZE - 1);
    std::vector<std::pair<int, int>> possiblePositions;
    
    for (int x = 0; x < LEVEL_SIZE; ++x) {
        for (int y = 0; y < LEVEL_SIZE; ++y) {
            if (m_levelMap[x][y] == EMPTY && countAdjacentRooms(x, y) == 1) {
                possiblePositions.emplace_back(x, y);
            }
        }
    }
    
    if (possiblePositions.size() < 3) {
        initializeLevel();
        generateRooms();
        return;
    }
    
    std::shuffle(possiblePositions.begin(), possiblePositions.end(), m_rng);
    
    m_levelMap[possiblePositions[0].first][possiblePositions[0].second] = TREASURE;
    m_levelMap[possiblePositions[1].first][possiblePositions[1].second] = SHOP;
    m_levelMap[possiblePositions[2].first][possiblePositions[2].second] = BOSS;
}

void LevelGenerator::generateRooms() {
    std::uniform_int_distribution<int> dist(0, LEVEL_SIZE - 1);
    std::uniform_real_distribution<float> roomChance(0.0f, 1.0f);
    
    int attempts = 0;
    const int MAX_ATTEMPTS = 1000;
    
    while (attempts < MAX_ATTEMPTS) {
        int x = dist(m_rng);
        int y = dist(m_rng);
        
        if (m_levelMap[x][y] == EMPTY && hasAdjacentRoom(x, y) && 
            roomChance(m_rng) < 0.3f && countRooms() < MAX_ROOMS) {
            m_levelMap[x][y] = ROOM;
        }
        attempts++;
    }
    
    if (countRooms() < MIN_ROOMS) {
        initializeLevel();
        generateRooms();
    }
}

std::vector<std::vector<int>> LevelGenerator::generateLevelMap() {
    generateRooms();
    placeSpecialRooms();
    return m_levelMap;
} 