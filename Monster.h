#pragma once
#include "SFML/Graphics.hpp"
#include "DamageManager.h"
#include "CharacterState.h"
#include "Animator.h"
#include "GameObject.h"
#include <map>
#include "PlayerPosition.h"

class Monster : public GameObject
{
public:
	Monster() : m_texture(), m_sprite(m_texture) {}
	virtual void update(float dt) {}
	virtual void draw(sf::RenderWindow* window);
	virtual sf::Sprite& getSprite() { return m_sprite; }
	virtual void setPosition(sf::Vector2f newPosiiton) { m_sprite.setPosition(newPosiiton); }
	void setRoomBounds(const sf::FloatRect& bounds);
	bool isPlayerInRange() const;

	sf::Vector2f getCenterPosition() { return m_collision.getGeometricCenter(); }
	sf::FloatRect getCollision() { return m_collision.getGlobalBounds(); }
	
	
	DamageManager MonsterDamage;
protected:
	virtual void loadTextures() {}

	int HP = 0;

	sf::Texture m_texture;
	std::map<CharacterState, sf::Texture> m_textures;
	sf::Sprite m_sprite;
	sf::RectangleShape m_collision;

	sf::FloatRect m_roomBounds;

	sf::Vector2f m_playerPosition;
	float m_detectionRadius = 300.f;

	float attackCooldown = 4.f;
};

class SlimeDefault : public Monster
{
public:
	SlimeDefault();
	virtual void update(float dt) override;

private:
	void chooseNewTargetPosition();
	void moveTowardsTarget(float dt, float timeToTarget = 0.f);
	bool isTargetReached() const;

	virtual void loadTextures() override;
private:
	CharacterState m_currentCharacterState = CharacterState::None;
	Animator m_animator;
	sf::Vector2f m_targetPosition;
	float m_speed = 120.f;
	float m_dashSpeed = 240.f;

	bool m_isChasingPlayer;

	bool m_isAttacking = false;

	sf::Vector2f m_startPosition;
	float m_totalTimeToTarget;
	bool m_isMovingToTarget;
	float m_elapsedTimeToTarget;
		
private:
	int MaxHP = 60;
	int HP = MaxHP;

	int damage = 10;
	int bodyDamage = 10;
};