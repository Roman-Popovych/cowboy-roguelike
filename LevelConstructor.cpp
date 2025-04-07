#include <iostream>
#include "LevelConstructor.h"
#include "LevelGenerator.h"
#include "Player.h"

#include "Monster.h"

LevelConstructor::LevelConstructor(sf::RenderWindow& window, LevelGenerator& levelGenerator) 
	: m_window(&window), m_levelGenerator(&levelGenerator)
	, m_rng(std::random_device()())
{
	m_levelMap = m_levelGenerator->getLevelMap();
	m_windowSizeX = m_window->getSize().x;
	m_windowSizeY = m_window->getSize().y;
}

void LevelConstructor::LevelConstruct(sf::View& view, Player* player)
{
	float totalWidth = m_levelGenerator->LEVEL_SIZE * GameConfig::screenWidth;
	float totalHeight = m_levelGenerator->LEVEL_SIZE * GameConfig::screenHeight;

	float centerX = totalWidth / 2.0f;
	float centerY = totalHeight / 2.0f;

	view.setCenter({ centerX, centerY });
	player->setPosition({ centerX, centerY });     

	if (GameConfig::isDebug)
	{
		std::cout << "view:\nx: " << centerX << "\ny: " << centerY << std::endl;
	}

	for (int i = 0; i < m_levelGenerator->LEVEL_SIZE; i++)
	{
		for (int j = 0; j < m_levelGenerator->LEVEL_SIZE; j++)
		{
			if (m_levelMap[i][j] == 1)
			{
				int roomWidth = getRandom(9, 15);
				int roomHeight = getRandom(7, 9);
				float posX = j * GameConfig::screenWidth;
				float posY = i * GameConfig::screenHeight;

				if (GameConfig::isDebug)
				{
					std::cout << "Generating room: " << roomWidth << "x" << roomHeight
						<< " at (" << posX << ", " << posY << ")\n";
				}
				std::map<AdjacentRoomPlace, std::pair<int, int>> adjacentRooms = findAdjacentRooms(i, j);

				Room* room = m_roomGenerator.generateRoom(roomWidth, roomHeight, posX, posY, adjacentRooms);

				room->addMonster(new SlimeDefault());

				m_roomArray.push_back(room);
				m_roomsByPosOnMap.emplace(std::make_pair(j, i), room);
			}
		}
	}
}

int LevelConstructor::getRandom(int min, int max)
{
	if (min % 2 == 0) ++min;
	if (max % 2 == 0) --max;

	std::uniform_int_distribution<int> dist(min / 2, max / 2);
	return dist(m_rng) * 2 + 1;
}

std::map<AdjacentRoomPlace, std::pair<int, int>> LevelConstructor::findAdjacentRooms(int i, int j)
{
	const std::vector<std::pair<int, int>> directions = {
		{0, -1}, {-1, 0}, {0, 1}, {1, 0}
	};

	const std::vector<AdjacentRoomPlace> directionEnums = {
		AdjacentRoomPlace::left, AdjacentRoomPlace::top,
		AdjacentRoomPlace::right, AdjacentRoomPlace::bottom
	};

	std::map<AdjacentRoomPlace, std::pair<int, int>> adjacentRooms;

	for (size_t d = 0; d < directions.size(); ++d)
	{
		int ni = i + directions[d].first;
		int nj = j + directions[d].second;

		if (ni < 0 || ni >= m_levelGenerator->LEVEL_SIZE || nj < 0 || nj >= m_levelGenerator->LEVEL_SIZE)
			continue;

		if (m_levelMap[ni][nj] == 1)
		{
			adjacentRooms[directionEnums[d]] = { ni, nj };
		}
	}

	return adjacentRooms;
}
