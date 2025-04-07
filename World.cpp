#include "World.h"
#include "Player.h"

World::World(sf::RenderWindow* window, Player* player)
    : m_window(window)
    , m_levelConstructor(*window, m_levelGenerator)
    , m_player(player)
{
    m_player->setWorld(this);
    m_collider.setPlayer(m_player);
    m_cameraView.setSize({ GameConfig::screenWidth, GameConfig::screenHeight });
    m_cameraView.setCenter({ GameConfig::screenWidth / 2, GameConfig::screenHeight / 2 });
}

World::~World()
{
    for (Room* room : m_rooms)
    {
        delete room;
    }
    m_rooms.clear();
}

void World::createLevel()
{
    m_levelMap = m_levelGenerator.generateLevelMap();
    m_levelConstructor.setLevelMap(m_levelMap);
    m_levelConstructor.LevelConstruct(m_cameraView, m_player);
    setRooms();
    m_roomsByPosOnMap = m_levelConstructor.getRoomsPosOnMap();
    setCurrentRoom(m_levelMap[0].size() / 2, m_levelMap.size() / 2);
    for (auto& room : m_rooms)
    {
        m_collider.addObject(room);
    }
}

void World::update(float dt)
{
    m_collider.update(dt);
    m_currentRoom->update(dt);
}

void World::draw()
{
    for (auto& room : m_rooms)
    {
        room->draw(*m_window);
    }
}

void World::handleInput(const sf::Event& event)
{
    
}

void World::setRooms()
{
    auto uniqueRooms = m_levelConstructor.getRooms();
    m_rooms.clear();
    m_rooms.reserve(uniqueRooms.size());

    for (auto& room : uniqueRooms) {
        m_rooms.push_back(room);
    }
}

void World::setCurrentRoom(int x, int y)
{
    m_currentRoom = m_roomsByPosOnMap.at({ x, y });
    m_currentRoomPosOnMap = std::make_pair(x, y);


    sf::Vector2f newCameraCenter = m_currentRoom->getCenterPosition();
    m_cameraView.setCenter(newCameraCenter);

    m_player->setPosition(newCameraCenter);
}

void World::moveToRoom(AdjacentRoomPlace direction)
{
    //switch (dir)
    //{
    //case AdjacentRoomPlace::left:
    //    setCurrentRoom(m_currentRoomPosOnMap.first - 1, m_currentRoomPosOnMap.second);
    //    break;
    //case AdjacentRoomPlace::top:
    //    setCurrentRoom(m_currentRoomPosOnMap.first, m_currentRoomPosOnMap.second - 1);
    //    break;
    //case AdjacentRoomPlace::right:
    //    setCurrentRoom(m_currentRoomPosOnMap.first + 1, m_currentRoomPosOnMap.second);
    //    break;
    //case AdjacentRoomPlace::bottom:
    //    setCurrentRoom(m_currentRoomPosOnMap.first, m_currentRoomPosOnMap.second + 1);
    //    break;
    //default:
    //    break;
    //}

    int x = m_currentRoomPosOnMap.first;
    int y = m_currentRoomPosOnMap.second;

    switch (direction)
    {
    case AdjacentRoomPlace::left:   x -= 1; break;
    case AdjacentRoomPlace::right:  x += 1; break;
    case AdjacentRoomPlace::top:    y -= 1; break;
    case AdjacentRoomPlace::bottom: y += 1; break;
    }

    if (m_roomsByPosOnMap.count({ x, y }) > 0)
    {
        setCurrentRoom(x, y);
    }
}
