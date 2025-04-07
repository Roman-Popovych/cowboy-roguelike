#pragma once
#include "AdjacentRoomPlace.h"
#include <map>
#include <utility>
#include <SFML/Graphics.hpp>
#include <optional>
#include "Room.h"
#include "GameConfig.h"


class RoomGenerator {
public:
    RoomGenerator();
    Room* generateRoom(int widthInSprites, int heightInSprites, float globalX, float globalY, std::map<AdjacentRoomPlace, std::pair<int, int>>& adjacentRooms);

private:
    void generateFloor(Room& room, float startX, float startY);
    void generateCorners(Room& room, float startX, float startY);
    void generateWalls(Room& room, float startX, float startY);
    void generateDoors(Room& room, float startX, float startY, std::map<AdjacentRoomPlace, std::pair<int, int>>& adjacentRooms);

    sf::Texture m_wallTexture;
    sf::Texture m_floorTexture;
    sf::Texture m_cornerTexture;
    sf::Texture m_doorTexture;

    sf::Sprite m_wallSprite;
    sf::Sprite m_floorSprite;
    sf::Sprite m_cornerSprite;
    sf::Sprite m_doorSprite;


    int m_spriteWidth;
    int m_spriteHeight;
};