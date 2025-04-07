#pragma once
#include <SFML/Graphics.hpp>

class GameObject
{
public:
    virtual void onCollision(GameObject* gameObject) {};

    virtual sf::FloatRect getGlobalBounds() const { return sf::FloatRect({ 0.f, 0.f }, { 0.f, 0.f }); }
};

