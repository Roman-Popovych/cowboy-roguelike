#pragma once
#include "SFML/Graphics.hpp"
#include "LevelConstructor.h"
#include "LevelGenerator.h"
#include "Collider.h"

class Player;

class World {
public:
    World(sf::RenderWindow* window, Player* player);
    ~World();

    void createLevel();
    void update(float dt);
    void draw();
    void handleInput(const sf::Event& event);

    //void addRoom(Room* room) { m_rooms.push_back(room); };
    void setRooms();
    void setCurrentRoom(int x, int y);
    Room* getCurrentRoom() { return m_currentRoom; }

    void moveToRoom(AdjacentRoomPlace dir);

    Player* getPlayer() { return m_player; }

    sf::View getView() const { return m_cameraView; }

    std::vector<std::vector<int>> getLevelMap() { return m_levelMap; }
    
    LevelGenerator m_levelGenerator;
private:
    sf::RenderWindow* m_window;
    LevelConstructor m_levelConstructor;

    std::vector<std::vector<int>> m_levelMap;
    std::vector<Room*> m_rooms;
    Room* m_currentRoom;
    std::pair<int, int> m_currentRoomPosOnMap;
    std::map<std::pair<int, int>, Room*> m_roomsByPosOnMap;

    sf::View m_cameraView;

    Player* m_player;
    Collider m_collider;
};
