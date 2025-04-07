#pragma once
#include "SFML/Graphics.hpp"
#include "CharacterState.h"
#include "AdjacentRoomPlace.h"
#include "GameObject.h"
#include "Animator.h"
#include "Collider.h"
#include "PlayerPosition.h"
#include "Bullet.h"

class World;
class Room;

class Player : public GameObject
{
public:
    Player(sf::RenderWindow* window);

    void setWorld(World* world);
    void setLevelMap(std::vector<std::vector<int>> levelMap);

    void update(float dt);
    void draw();

    void updateHPBar();

    const sf::Sprite& getSprite() const { return m_sprite; }
    sf::FloatRect getGlobalBounds() const override
    {
        return m_sprite.getGlobalBounds();
    }

    void onCollision(GameObject* gameObject) override;
    sf::Clock m_timeToIdleAnimation;

    sf::Vector2f getPosition() { return m_position; }

    sf::Vector2f getCenterPosition() {
        return sf::Vector2f(
            m_collision.getPosition().x + m_collision.getGlobalBounds().size.x / 2.f,
            m_collision.getPosition().y + m_collision.getGlobalBounds().size.y / 2.f
        );
    }

    void setPosition(sf::Vector2f newPosition) { m_position = newPosition; m_sprite.setPosition(m_position); }

    bool checkCollisionWithWalls(const sf::Vector2f& newPosition);

private:
    void shoot(const sf::Vector2f& direction);
    void handleInput(float dt);
    void loadTextures();

private:
    sf::RenderWindow* m_window;

    sf::Texture m_texture;
    std::map<CharacterState, sf::Texture> m_textures;

    sf::Sprite m_sprite;
    sf::RectangleShape  m_collision;

    sf::Vector2f m_position;

    float m_speed = 400.0f;
    Animator m_animator;
    CharacterState m_currentCharacterState = CharacterState::None;

    World* m_world;
    std::vector<std::vector<int>> m_levelMap;

    float m_damageCooldown = 1.2f; 
    float m_damageTimer = 0.f;
    bool m_isDamageReceived = false;


    sf::RectangleShape m_hpBarBackground;
    sf::RectangleShape m_hpBarFill;

    float m_hpBarMaxWidth = 200.f;
    float m_hpBarMaxHeight = 40.f;

private:
    int m_maxHP = 100;
    int m_currentHP = m_maxHP;

private:
    std::vector<Bullet> m_bullets;
    float m_bulletSpeed = 500.f;
    float m_shootCD = 0.5f;
    float m_shootTimer = m_shootCD;
};

