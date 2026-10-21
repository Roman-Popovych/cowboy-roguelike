#include "RoomGenerator.h"
#include "TextureLoader.h"
#include <iostream>
#include <memory>


RoomGenerator::RoomGenerator()
    : m_wallTexture(), m_floorTexture(), m_cornerTexture(), m_doorTexture(),
    m_wallSprite(m_wallTexture), m_floorSprite(m_floorTexture), m_cornerSprite(m_cornerTexture), m_doorSprite(m_doorTexture)
{
    m_wallTexture = loadTexture("resources/WorldResources/Dungeon/Dungeon_Tileset.png", Placeholders::tileset);
    m_floorTexture = loadTexture("resources/WorldResources/Dungeon/Dungeon_Tileset.png", Placeholders::tileset);
    m_cornerTexture = loadTexture("resources/WorldResources/Dungeon/cornerWall.png", Placeholders::corner);
    m_doorTexture = loadTexture("resources/WorldResources/Dungeon/Dungeon_Tileset.png", Placeholders::tileset);

    m_wallSprite.setTexture(m_wallTexture);
    m_floorSprite.setTexture(m_floorTexture);
    m_cornerSprite.setTexture(m_cornerTexture);
    m_doorSprite.setTexture(m_doorTexture);

    m_spriteWidth = m_floorTexture.getSize().x / 10;
    m_spriteHeight = m_floorTexture.getSize().y / 10;
}


Room* RoomGenerator::generateRoom(int widthInSprites, int heightInSprites, float globalX, float globalY, std::map<AdjacentRoomPlace, std::pair<int, int>>& adjacentRooms)
{
    Room* room = new Room(widthInSprites, heightInSprites);

    float startX = globalX + (GameConfig::screenWidth - (widthInSprites * GameConfig::defaultSpriteSize)) / 2.0f;
    float startY = globalY + (GameConfig::screenHeight - (heightInSprites * GameConfig::defaultSpriteSize)) / 2.0f;
    
    generateFloor(*room, startX, startY);
    generateWalls(*room, startX, startY);
    generateCorners(*room, startX, startY);
    generateDoors(*room, startX, startY, adjacentRooms);
    
    return room;
}

void RoomGenerator::generateFloor(Room& room, float startX, float startY)
{
    float scaleX = static_cast<float>(GameConfig::defaultSpriteSize) / m_spriteWidth;
    float scaleY = static_cast<float>(GameConfig::defaultSpriteSize) / m_spriteHeight;
    m_floorSprite.setScale({ scaleX, scaleY });

    m_floorSprite.setTextureRect(sf::IntRect({ m_spriteWidth, m_spriteHeight }, { m_spriteWidth, m_spriteHeight }));

    m_floorSprite.setOrigin({m_spriteWidth / 2.0f, m_spriteHeight / 2.0f});

    for (int y = 0; y < room.getHeightInSprites(); ++y)
    {
        for (int x = 0; x < room.getWidthInSprites(); ++x)
        {
            m_floorSprite.setPosition({
                startX + static_cast<float>(x * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f,
                startY + static_cast<float>(y * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f
            });
            room.addSprite(m_floorSprite);
        }
    }
}

void RoomGenerator::generateCorners(Room& room, float startX, float startY)
{
    float scaleX = static_cast<float>(GameConfig::defaultSpriteSize) / m_spriteWidth;
    float scaleY = static_cast<float>(GameConfig::defaultSpriteSize) / m_spriteHeight;
    m_cornerSprite.setScale({scaleX, scaleY});

    m_cornerSprite.setTextureRect(sf::IntRect({ 0, 0 }, { m_spriteWidth , m_spriteHeight }));

    m_cornerSprite.setOrigin({m_spriteWidth / 2.0f, m_spriteHeight / 2.0f});

    m_cornerSprite.setPosition({
        startX + GameConfig::defaultSpriteSize / 2.0f,
        startY + GameConfig::defaultSpriteSize / 2.0f
    });
    m_cornerSprite.setRotation(sf::degrees(-90));
    room.addSprite(m_cornerSprite);

    m_cornerSprite.setPosition({
        startX + static_cast<float>((room.getWidthInSprites() - 1) * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f,
        startY + GameConfig::defaultSpriteSize / 2.0f
    });
    m_cornerSprite.setRotation(sf::degrees(0));
    room.addSprite(m_cornerSprite);

    m_cornerSprite.setPosition({
        startX + GameConfig::defaultSpriteSize / 2.0f,
        startY + static_cast<float>((room.getHeightInSprites() - 1) * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f
    });
    m_cornerSprite.setRotation(sf::degrees(180));
    room.addSprite(m_cornerSprite);

    m_cornerSprite.setPosition({
        startX + static_cast<float>((room.getWidthInSprites() - 1) * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f,
        startY + static_cast<float>((room.getHeightInSprites() - 1) * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f
    });
    m_cornerSprite.setRotation(sf::degrees(90));
    room.addSprite(m_cornerSprite);
}

void RoomGenerator::generateWalls(Room& room, float startX, float startY)
{
    float scaleX = static_cast<float>(GameConfig::defaultSpriteSize) / m_spriteWidth;
    float scaleY = static_cast<float>(GameConfig::defaultSpriteSize) / m_spriteHeight;
    m_wallSprite.setScale({scaleX, scaleY});

    m_wallSprite.setTextureRect(sf::IntRect({ m_spriteWidth, 0 }, { m_spriteWidth, m_spriteHeight }));

    m_wallSprite.setOrigin({m_spriteWidth / 2.0f, m_spriteHeight / 2.0f});

    for (int x = 1; x < room.getWidthInSprites() - 1; ++x)
    {
        m_wallSprite.setPosition({
            startX + static_cast<float>(x * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f,
            startY + GameConfig::defaultSpriteSize / 2.0f
        });
        m_wallSprite.setRotation(sf::degrees(0));
        room.addSprite(m_wallSprite);
        room.addWalls(m_wallSprite);
    }

    for (int x = 1; x < room.getWidthInSprites() - 1; ++x)
    {
        m_wallSprite.setPosition({
            startX + static_cast<float>(x * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f,
            startY + static_cast<float>((room.getHeightInSprites() - 1) * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f
        });
        m_wallSprite.setRotation(sf::degrees(180));
        room.addSprite(m_wallSprite);
        room.addWalls(m_wallSprite);
    }

    for (int y = 1; y < room.getHeightInSprites() - 1; ++y)
    {
        m_wallSprite.setPosition({
            startX + GameConfig::defaultSpriteSize / 2.0f,
            startY + static_cast<float>(y * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f
        });
        m_wallSprite.setRotation(sf::degrees(-90));
        room.addSprite(m_wallSprite);
        room.addWalls(m_wallSprite);
    }

    for (int y = 1; y < room.getHeightInSprites() - 1; ++y)
    {
        m_wallSprite.setPosition({
            startX + static_cast<float>((room.getWidthInSprites() - 1) * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f,
            startY + static_cast<float>(y * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f
        });
        m_wallSprite.setRotation(sf::degrees(90));
        room.addSprite(m_wallSprite);
        room.addWalls(m_wallSprite);
    }
}

void RoomGenerator::generateDoors(Room& room, float startX, float startY, std::map<AdjacentRoomPlace, std::pair<int, int>>& adjacentRooms)
{
    float scaleX = static_cast<float>(GameConfig::defaultSpriteSize) / m_spriteWidth;
    float scaleY = static_cast<float>(GameConfig::defaultSpriteSize) / m_spriteHeight;
    m_doorSprite.setScale({ scaleX, scaleY });

    m_doorSprite.setTextureRect(sf::IntRect({ m_spriteWidth * 7, m_spriteHeight * 3 }, { m_spriteWidth, m_spriteHeight }));

    m_doorSprite.setOrigin({ m_spriteWidth / 2.0f, m_spriteHeight / 2.0f });

    for (const auto& adjacent : adjacentRooms)
    {
        switch (adjacent.first)
        {
        case AdjacentRoomPlace::left:
            m_doorSprite.setPosition({
                startX + GameConfig::defaultSpriteSize / 2.0f,
                startY + (room.getHeightInSprites() / 2.0f) * GameConfig::defaultSpriteSize
                });
            m_doorSprite.setRotation(sf::degrees(-90));
            break;

        case AdjacentRoomPlace::top:
            m_doorSprite.setPosition({
                startX + (room.getWidthInSprites() / 2.0f) * GameConfig::defaultSpriteSize,
                startY + GameConfig::defaultSpriteSize / 2.0f
                });
            m_doorSprite.setRotation(sf::degrees(0));
            break;

        case AdjacentRoomPlace::right:
            m_doorSprite.setPosition({
                startX + static_cast<float>((room.getWidthInSprites() - 1) * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f,
                startY + (room.getHeightInSprites() / 2.0f) * GameConfig::defaultSpriteSize
                });
            m_doorSprite.setRotation(sf::degrees(90));
            break;

        case AdjacentRoomPlace::bottom:
            m_doorSprite.setPosition({
                startX + (room.getWidthInSprites() / 2.0f) * GameConfig::defaultSpriteSize,
                startY + static_cast<float>((room.getHeightInSprites() - 1) * GameConfig::defaultSpriteSize) + GameConfig::defaultSpriteSize / 2.0f
                });
            m_doorSprite.setRotation(sf::degrees(180));
            break;
        }

        room.addSprite(m_doorSprite);
        room.addDoorTrigger(m_doorSprite, adjacent.first);
    }

    if (GameConfig::isDebug)
    {
        std::cout << "Added door at: (" << m_doorSprite.getPosition().x << ", " << m_doorSprite.getPosition().y << ")\n";

    }
}
