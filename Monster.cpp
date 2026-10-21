#include "Monster.h"
#include "TextureLoader.h"
#include <iostream>
#include "GameConfig.h"
#include <random>

void Monster::draw(sf::RenderWindow* window)
{
	window->draw(m_sprite);
	if (GameConfig::isDebug)
	{
		window->draw(m_collision);
	}
}

void Monster::setRoomBounds(const sf::FloatRect& bounds)
{
	m_roomBounds = bounds;
}

bool Monster::isPlayerInRange() const
{
	sf::Vector2f playerPos = PlayerPosition::getPosition();

	sf::Vector2f monsterCenter = m_collision.getPosition() + m_collision.getSize() / 2.f;

	float dx = playerPos.x - monsterCenter.x;
	float dy = playerPos.y - monsterCenter.y;

	float distanceSquared = dx * dx + dy * dy;

	if (GameConfig::isDebug)
	{
		static sf::Clock clock;
		if (clock.getElapsedTime().asSeconds() >= 3.f)
		{
			std::cout << "Player Pos: (" << playerPos.x << ", " << playerPos.y << ")" << std::endl;
			std::cout << "Monster Center: (" << monsterCenter.x << ", " << monsterCenter.y << ")" << std::endl;
			std::cout << "Delta: (" << dx << ", " << dy << ")" << std::endl;
			std::cout << "Distance Squared: " << distanceSquared << std::endl;

			clock.restart();
		}
	}


	return distanceSquared <= m_detectionRadius * m_detectionRadius;
}

SlimeDefault::SlimeDefault()
{
	loadTextures();
	MonsterDamage.bodyDamage = bodyDamage;
	MonsterDamage.damage = damage;

	m_texture = loadTexture("resources/Enemies/Monster/SlimeDefault/Idle/Slime1_Idle_full.png", Placeholders::slimeIdle);

	m_animator.addAnimation(AnimationType::None, Animation(&m_textures[CharacterState::Idle], 6, 0, 9999.f, 4));

	m_animator.addAnimation(AnimationType::MoveDown, Animation(&m_textures[CharacterState::Move], 8, 0, 0.2f, 4));
	m_animator.addAnimation(AnimationType::MoveUp, Animation(&m_textures[CharacterState::Move], 8, 1, 0.2f, 4));
	m_animator.addAnimation(AnimationType::MoveLeft, Animation(&m_textures[CharacterState::Move], 8, 2, 0.2f, 4));
	m_animator.addAnimation(AnimationType::MoveRight, Animation(&m_textures[CharacterState::Move], 8, 3, 0.2f, 4));

	m_animator.addAnimation(AnimationType::DashDown, Animation(&m_textures[CharacterState::Dash], 8, 0, 0.1f, 4));
	m_animator.addAnimation(AnimationType::DashUp, Animation(&m_textures[CharacterState::Dash], 8, 1, 0.1f, 4));
	m_animator.addAnimation(AnimationType::DashLeft, Animation(&m_textures[CharacterState::Dash], 8, 2, 0.1f, 4));
	m_animator.addAnimation(AnimationType::DashRight, Animation(&m_textures[CharacterState::Dash], 8, 3, 0.1f, 4));

	m_animator.addAnimation(AnimationType::Attack, Animation(&m_textures[CharacterState::Attack], 10, 0, 0.2f, 4));

	m_sprite.setTexture(m_texture);

	int sizeX = m_texture.getSize().x / 6;
	int sizeY = m_texture.getSize().y / 4;

	m_sprite.setTextureRect(sf::IntRect({ 0, 0 }, { sizeX, sizeY }));
	m_sprite.scale({ 3, 3 });


	m_collision.setSize({ m_sprite.getGlobalBounds().size.x / 6, m_sprite.getGlobalBounds().size.y / 6 });
	m_collision.setFillColor(sf::Color(188, 0, 0, 150));

	m_targetPosition = { 0, 0 };
}

void SlimeDefault::update(float dt)
{
	float collisionPosX = m_sprite.getPosition().x + m_sprite.getGlobalBounds().size.x / 2 - m_collision.getSize().x / 2;
	float collisionPosY = m_sprite.getPosition().y + m_sprite.getGlobalBounds().size.y / 2 - m_collision.getSize().y / 2;
	m_collision.setPosition({ collisionPosX, collisionPosY });

	if (attackCooldown > 0.0f)
	{
		attackCooldown -= dt;
	}

	if (isPlayerInRange() && attackCooldown <= 0.0f) {
		if (!m_isAttacking)
		{
			m_targetPosition = PlayerPosition::getPosition();
			m_isAttacking = true;
		}

		sf::Vector2f movementDirection = m_targetPosition - getCenterPosition();

		if (std::abs(movementDirection.x) > std::abs(movementDirection.y)) {
			if (movementDirection.x > 0) {
				m_currentCharacterState = CharacterState::Dash;
				m_sprite.setTexture(m_textures[m_currentCharacterState]);
				m_animator.setAnimation(AnimationType::DashRight);
			}
			else {
				m_currentCharacterState = CharacterState::Dash;
				m_sprite.setTexture(m_textures[m_currentCharacterState]);
				m_animator.setAnimation(AnimationType::DashLeft);
			}
		}
		else {
			if (movementDirection.y > 0) {
				m_currentCharacterState = CharacterState::Dash;
				m_sprite.setTexture(m_textures[m_currentCharacterState]);
				m_animator.setAnimation(AnimationType::DashDown);
			}
			else {
				m_currentCharacterState = CharacterState::Dash;
				m_sprite.setTexture(m_textures[m_currentCharacterState]);
				m_animator.setAnimation(AnimationType::DashUp);
			}
		}


		moveTowardsTarget(dt, 0.8);
		if (isTargetReached())
		{
			m_isAttacking = false;
			m_isMovingToTarget = false;
			attackCooldown = 4.0f;
		}


		m_animator.update(dt, m_sprite);
		return;
	}


	if (isTargetReached() || m_targetPosition == sf::Vector2f{ 0, 0 }) {
		chooseNewTargetPosition();
	}

	moveTowardsTarget(dt);

	sf::Vector2f movementDirection = m_targetPosition - m_sprite.getPosition();

	if (std::abs(movementDirection.x) > std::abs(movementDirection.y)) {
		if (movementDirection.x > 0) {
			m_currentCharacterState = CharacterState::Move;
			m_sprite.setTexture(m_textures[m_currentCharacterState]);
			m_animator.setAnimation(AnimationType::MoveRight);
		}
		else {
			m_currentCharacterState = CharacterState::Move;
			m_sprite.setTexture(m_textures[m_currentCharacterState]);
			m_animator.setAnimation(AnimationType::MoveLeft);
		}
	}
	else {
		if (movementDirection.y > 0) {
			m_currentCharacterState = CharacterState::Move;
			m_sprite.setTexture(m_textures[m_currentCharacterState]);
			m_animator.setAnimation(AnimationType::MoveDown);
		}
		else {
			m_currentCharacterState = CharacterState::Move;
			m_sprite.setTexture(m_textures[m_currentCharacterState]);
			m_animator.setAnimation(AnimationType::MoveUp);
		}
	}

	m_animator.update(dt, m_sprite);
}

void SlimeDefault::chooseNewTargetPosition()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> disX(m_roomBounds.position.x, m_roomBounds.position.x + m_roomBounds.size.x - m_sprite.getGlobalBounds().size.x);
	std::uniform_real_distribution<> disY(m_roomBounds.position.y, m_roomBounds.position.y + m_roomBounds.size.y - m_sprite.getGlobalBounds().size.y);

	float x = disX(gen);
	float y = disY(gen);

	m_targetPosition = { x, y };

	if (GameConfig::isDebug)
		std::cout << "Target Position: " << m_targetPosition.x << ", " << m_targetPosition.y << std::endl;
}

void SlimeDefault::moveTowardsTarget(float dt, float timeToTarget)
{
	if (timeToTarget <= 0.f) {
		sf::Vector2f direction = m_targetPosition - m_sprite.getPosition();
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		if (distance > 1.f) {
			direction /= distance;
			m_sprite.move(direction * m_speed * dt);
		}
		return;
	}


	sf::Vector2f diff = m_targetPosition - m_sprite.getPosition();
	float distanceToTarget = std::sqrt(diff.x * diff.x + diff.y * diff.y);

	if (!m_isMovingToTarget && distanceToTarget > 1.f) {
		m_startPosition = m_sprite.getPosition();
		m_elapsedTimeToTarget = 0.f;
		m_totalTimeToTarget = timeToTarget;
		m_isMovingToTarget = true;
	}

	if (m_isMovingToTarget) {
		m_elapsedTimeToTarget += dt;
		float progress = std::min(m_elapsedTimeToTarget / m_totalTimeToTarget, 1.f);

		sf::Vector2f newPosition = m_startPosition + (m_targetPosition - m_startPosition) * progress;
		m_sprite.setPosition(newPosition);

		if (progress >= 1.f) {
			m_isMovingToTarget = false;
		}
	}
}

bool SlimeDefault::isTargetReached() const
{
	const float tolerance = 5.0f;

	return (std::abs(m_sprite.getPosition().x - m_targetPosition.x) < tolerance &&
		std::abs(m_sprite.getPosition().y - m_targetPosition.y) < tolerance);
}

void SlimeDefault::loadTextures()
{
	m_textures[CharacterState::None] = loadTexture("resources/Enemies/Monster/SlimeDefault/Idle/Slime1_Idle_full.png", Placeholders::slimeIdle);
	m_textures[CharacterState::Move] = loadTexture("resources/Enemies/Monster/SlimeDefault/Walk/Slime1_Walk_full.png", Placeholders::slimeWalk);
	m_textures[CharacterState::Dash] = loadTexture("resources/Enemies/Monster/SlimeDefault/Run/Slime1_Run_full.png", Placeholders::slimeRun);
	m_textures[CharacterState::Attack] = loadTexture("resources/Enemies/Monster/SlimeDefault/Attack/Slime1_Attack_full.png", Placeholders::slimeAttack);
}
