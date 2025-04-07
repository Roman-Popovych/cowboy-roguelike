#pragma once
#include "SFML/Graphics.hpp"

class PlayerPosition
{
public:
	static sf::Vector2f getPosition() { return position; }

	static void updatePosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
	static void updatePosition(sf::Vector2f pos)
	{
		position = pos;
	}
private:
	static sf::Vector2f position;
};