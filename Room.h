#pragma once
#include "AdjacentRoomPlace.h"
#include "GameConfig.h"
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Monster.h"


#include <iostream>

struct DoorTrigger {
    sf::Sprite sprite;
    AdjacentRoomPlace direction;
};

class Room : public GameObject
{
public:
    // Конструктор за замовчуванням
    Room() : m_widthInSprites(12), m_heightInSprites(8), 
             m_spriteWidth(GameConfig::defaultSpriteSize), 
             m_spriteHeight(GameConfig::defaultSpriteSize) {}

    // Основний конструктор
    Room(int widthInSprites, int heightInSprites)
        : m_widthInSprites(widthInSprites)
        , m_heightInSprites(heightInSprites)
        , m_spriteWidth(GameConfig::defaultSpriteSize)
        , m_spriteHeight(GameConfig::defaultSpriteSize) {}
    ~Room() 
    {
        for (auto monster : m_monsters) {
            delete monster;
        }
        m_monsters.clear();
    };

    void addSprite(const sf::Sprite& sprite)
    {
        m_sprites.push_back(sprite);
    }
    void addDoorTrigger(const sf::Sprite& sprite, AdjacentRoomPlace direction)
    {
        DoorTrigger doorTrigger{ sprite, direction };
        m_doorTriggers.push_back(doorTrigger);
    }
    void addWalls(const sf::Sprite& sprite)
    {
        m_walls.push_back(sprite);
    }

    std::vector<DoorTrigger>& getDoorTriggers()
    {
        return m_doorTriggers;
    }

    void update(float dt)
    {
        for (const auto& monster : m_monsters)
        {
            monster->update(dt);
        }
    }

    void draw(sf::RenderWindow& window)
    {
        for (const auto& sprite : m_sprites)
        {
            window.draw(sprite);
        }
        for (const auto& monster : m_monsters)
        {
            monster->draw(&window);
        }
    }
    
    virtual void onCollision(GameObject* gameObject) override
    {

    }

    sf::Vector2f getCenterPosition() const
    {
        // Обчислюємо центр кімнати, знаючи її розміри в спрайтах
        float centerX = (m_widthInSprites * m_spriteWidth) / 2.0f;
        float centerY = (m_heightInSprites * m_spriteHeight) / 2.0f;

        // Додаємо глобальну позицію до обчислення центру
        sf::Vector2f globalCenter = { centerX + m_sprites[0].getPosition().x, centerY + m_sprites[0].getPosition().y };

        return globalCenter;
    }

    int getWidthInSprites() const { return m_widthInSprites; }
    int getHeightInSprites() const { return m_heightInSprites; }

    sf::FloatRect getGlobalBounds() const
    {
        if (m_sprites.empty())
        {
            return sf::FloatRect{ {0.f, 0.f}, {0.f, 0.f} };
        }

        sf::Vector2f min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
        sf::Vector2f max = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };


        for (const auto& obj : m_sprites)
        {
            sf::FloatRect bounds = obj.getGlobalBounds();
            min.x = std::min(min.x, bounds.position.x);
            min.y = std::min(min.y, bounds.position.y);
            max.x = std::max(max.x, bounds.position.x + bounds.size.x);
            max.y = std::max(max.y, bounds.position.y + bounds.size.y);
        }

        return sf::FloatRect(min, max - min);
    }

    sf::Vector2f getDoorPosition(AdjacentRoomPlace direction) const
    {
        for (const auto& trigger : m_doorTriggers)
        {
            if (trigger.direction == direction)
            {
                return trigger.sprite.getPosition();
            }
        }

        std::cout << "No matching door trigger found, returning center position.\n";
        return getCenterPosition();
    }

    std::vector<sf::Vector2f> getWallsPosition() const 
    {
        std::vector<sf::Vector2f> wallPositions;
        for (const auto& wall : m_walls) {
            wallPositions.push_back(wall.getPosition());
        }
        return wallPositions;
    }

    std::vector<sf::Sprite> getWalls() const
    {
        std::vector<sf::Sprite> walls;

        for (const auto& wall : m_walls) {
            bool isDoorAtPosition = false;

            for (const auto& doorTrigger : m_doorTriggers) {
                if (wall.getGlobalBounds().findIntersection(doorTrigger.sprite.getGlobalBounds())) {
                    isDoorAtPosition = true;
                    break;
                }
            }

            if (!isDoorAtPosition) {
                walls.push_back(wall);
            }
        }

        return walls;
    }

    void addMonster(Monster* monster)
    {
        m_monsters.push_back(monster);
        monster->setPosition(getCenterPosition());

        sf::FloatRect bounds;
        //bounds.position.x = m_sprites[0].getPosition().x + m_spriteWidth;
        bounds.position.x = m_sprites[0].getGlobalBounds().position.x + m_spriteWidth;
        bounds.position.y = m_sprites[0].getGlobalBounds().position.y + m_spriteHeight;
        bounds.size.x = (m_widthInSprites - 2) * m_spriteWidth;
        bounds.size.y = (m_heightInSprites - 2) * m_spriteHeight;

        monster->setRoomBounds(bounds);
    }

    std::vector<Monster*> getMonsters() { return m_monsters; }

private:
    int m_widthInSprites;
    int m_heightInSprites;
    int m_spriteWidth;
    int m_spriteHeight;
    std::vector<sf::Sprite> m_sprites;

    std::vector<DoorTrigger> m_doorTriggers;
    std::vector<sf::Sprite> m_walls;

    std::vector<Monster*> m_monsters;
};
