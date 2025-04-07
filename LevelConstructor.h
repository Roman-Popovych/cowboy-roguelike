#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "RoomManager.h"
#include "RoomGenerator.h"

#include "AdjacentRoomPlace.h"

class LevelGenerator;
class Player;

class LevelConstructor
{
public:
	LevelConstructor(sf::RenderWindow& window, LevelGenerator& levelGenerator);
	void LevelConstruct(sf::View& view, Player* player);

	int getRandom(int min, int max);

	void setLevelMap(const std::vector<std::vector<int>>& map) { m_levelMap = map; }

	std::map<AdjacentRoomPlace, std::pair<int, int>> findAdjacentRooms(int i, int j);

	std::vector<Room*> getRooms() { return std::move(m_roomArray); }
	std::map<std::pair<int, int>, Room*> getRoomsPosOnMap() { return m_roomsByPosOnMap; }

private:
	std::vector<std::vector<int>> m_levelMap;
	LevelGenerator* m_levelGenerator;

	RoomGenerator m_roomGenerator;

	std::vector<Room*> m_roomArray;
	std::map<std::pair<int, int>, Room*> m_roomsByPosOnMap;

	sf::RenderWindow* m_window;

	float m_windowSizeX;
	float m_windowSizeY;

	std::mt19937 m_rng;
};