#include "SFML/Graphics.hpp"
#include "Collider.h"
#include "Player.h"
#include "World.h"

std::vector<GameObject*> Collider::m_objects = {};
Player* Collider::m_player = nullptr;

void Collider::update(float dt)
{
    if (!m_player) return;

    sf::FloatRect playerBounds = m_player->getGlobalBounds();

    for (GameObject* obj : m_objects)
    {
        if (!obj) continue;

        sf::FloatRect objBounds = obj->getGlobalBounds();

        if (playerBounds.findIntersection(objBounds))
        {
            m_player->onCollision(obj);
            obj->onCollision(m_player);
        }
    }
}