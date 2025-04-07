#pragma once
#include <vector>
#include "GameObject.h"

class Player;

class Collider
{
public:
    Collider() {}

    void addObject(GameObject* obj) { m_objects.push_back(obj); }
    void update(float dt);

    static void setPlayer(Player* player) { m_player = player; }
    static void clearObjects() { m_objects.clear(); }

private:
    static std::vector<GameObject*> m_objects;
    static Player* m_player;
};