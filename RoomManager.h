//#pragma once
//#include <SFML/Graphics.hpp>
//#include <map>
//#include "Room.h"
//#include "RoomGenerator.h"
//
//class RoomManager
//{
//public:
//    RoomManager(sf::RenderWindow& window);
//    //void generateNewLevel();
//    //void handleInput(const sf::Event& event);
//    void createRoom(int widthInSprites, int heightInSprites, float globalX, float globalY, int mapPosX, int mapPosY);
//    void update(float deltaTime);
//    void draw();
//
//private:
//    //void moveToRoom(int dx, int dy);
//    bool isValidMove(int dx, int dy) const;
//
//    sf::RenderWindow& m_window;
//    RoomGenerator m_roomGenerator;
//    std::map<std::pair<int, int>, Room> m_rooms;
//    std::pair<int, int> m_currentRoomIndex;
//}; 