#include "Player.h"
#include <iostream>
#include "GameConfig.h"
#include "Room.h"
#include "World.h"


Player::Player(sf::RenderWindow* window)
    : m_window(window)
    , m_texture("resources/playerCharacters/Cowboy/cowboy_idle_spritesheet.png")
    , m_sprite(m_texture)
{
    loadTextures();

    m_sprite.setScale({ 6, 6 });
    m_sprite.setTextureRect(sf::IntRect({ 0, 0 }, { static_cast<int>(m_texture.getSize().x) / 4, static_cast<int>(m_texture.getSize().y) }));

    const sf::Vector2u windowSize = m_window->getSize();

    m_sprite.setPosition(sf::Vector2f{ windowSize.x / 2.0f,
        windowSize.y / 2.0f });

    m_position = m_sprite.getPosition();


    m_animator.addAnimation(AnimationType::None, Animation(&m_texture, 4, 0, 9999.f));
    m_animator.addAnimation(AnimationType::Idle, Animation(&m_texture, 4, 0, 0.2f));
    m_animator.addAnimation(AnimationType::Move, Animation(&m_texture, 4, 0, 0.2f));
    m_animator.addAnimation(AnimationType::Attack, Animation(&m_texture, 2, 0, 0.2f));

    m_animator.setAnimation(AnimationType::Idle);


    m_collision.setSize(sf::Vector2f(m_sprite.getGlobalBounds().size.x * 0.33f, m_sprite.getGlobalBounds().size.y * 0.33f));
    
    m_collision.setFillColor(sf::Color(255, 0, 0, 100));

    sf::Vector2f spritePos = m_sprite.getPosition();
    sf::Vector2f rectSize = m_collision.getSize();

    m_collision.setPosition({
        spritePos.x + rectSize.x,
        spritePos.y + rectSize.y + 12
        });
   

    m_hpBarBackground.setSize({ m_hpBarMaxWidth, m_hpBarMaxHeight }); 
    m_hpBarBackground.setFillColor(sf::Color::Red);
    m_hpBarBackground.setPosition({ 20.f, 20.f });

    m_hpBarFill.setSize({ m_hpBarMaxWidth, m_hpBarMaxHeight });
    m_hpBarFill.setFillColor(sf::Color::Green);
    m_hpBarFill.setPosition({ 20.f, 20.f });
}

void Player::setWorld(World* world)
{
    m_world = world;
    setLevelMap(m_world->getLevelMap());
}

void Player::setLevelMap(std::vector<std::vector<int>> levelMap)
{
    m_levelMap = levelMap;
}

void Player::update(float dt)
{
    for (auto& bullet : m_bullets)
        bullet.update(dt);

    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(), [](const Bullet& b) {
            return b.getBounds().position.x > GameConfig::screenWidth ||
                b.getBounds().position.x + b.getBounds().size.x < 0 ||
                b.getBounds().position.y > GameConfig::screenHeight ||
                b.getBounds().position.y + b.getBounds().size.y < 0;
            }),
        m_bullets.end()
    );



    handleInput(dt);
    m_animator.update(dt, m_sprite);

    m_sprite.setPosition(m_position);


    sf::Vector2f spritePos = m_sprite.getPosition();
    sf::Vector2f rectSize = m_collision.getSize();

    m_collision.setPosition({
        spritePos.x + rectSize.x,
        spritePos.y + rectSize.y + 12
        });

    PlayerPosition::updatePosition(getCenterPosition());

    if (m_isDamageReceived)
    {
        m_damageTimer += dt;
        if (m_damageTimer >= m_damageCooldown)
        {
            m_isDamageReceived = false;
            m_damageTimer = 0.f;
        }
    }
    updateHPBar();
}

void Player::updateHPBar()
{
    float hpPercent = static_cast<float>(m_currentHP) / m_maxHP;
    m_hpBarFill.setSize({ m_hpBarMaxWidth * hpPercent, m_hpBarFill.getSize().y });
}



void Player::draw()
{
    m_window->draw(m_sprite);
    if (GameConfig::isDebug)
        m_window->draw(m_collision);

    for (auto& bullet : m_bullets)
        bullet.draw(*m_window);

    m_window->setView(m_window->getDefaultView());
    m_window->draw(m_hpBarBackground);
    m_window->draw(m_hpBarFill);
    m_window->setView(m_world->getView());

}

void Player::handleInput(float dt)
{
    float speed = m_speed * dt;
    sf::Vector2f movement(0.f, 0.f);
    CharacterState newCharacterState = CharacterState::None;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {

        movement.x -= 1.f;
        newCharacterState = CharacterState::MoveLeft;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        movement.x += 1.f;
        newCharacterState = CharacterState::MoveRight;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        movement.y -= 1.f;
        newCharacterState = CharacterState::MoveUp;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        movement.y += 1.f;
        newCharacterState = CharacterState::MoveDown;
    }


    if (m_shootTimer > 0.f)
        m_shootTimer -= dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        newCharacterState = CharacterState::AttackLeft;
        shoot({ -1.f, 0.f });
        m_shootTimer = m_shootCD;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        newCharacterState = CharacterState::AttackRight;
        shoot({ 1.f, 0.f });
        m_shootTimer = m_shootCD;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        newCharacterState = CharacterState::AttackUp;
        shoot({ 0.f, -1.f });
        m_shootTimer = m_shootCD;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        newCharacterState = CharacterState::AttackDown;
        shoot({ 0.f, 1.f });
        m_shootTimer = m_shootCD;
    }   

    if (movement.x != 0.f || movement.y != 0.f) {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement /= length;
    }

    sf::Vector2f newPosition = m_position + movement * speed;

    if (!checkCollisionWithWalls({ m_position.x + movement.x * speed, m_position.y })) {
        m_position.x = newPosition.x;
    }

    if (!checkCollisionWithWalls({ m_position.x, m_position.y + movement.y * speed })) {
        m_position.y = newPosition.y;
    }

    m_sprite.setPosition(m_position);

    if (newCharacterState != CharacterState::None) {
        m_timeToIdleAnimation.restart();
        if (newCharacterState != m_currentCharacterState) {
            m_currentCharacterState = newCharacterState;
            if (m_textures.contains(m_currentCharacterState)) {
                m_sprite.setTexture(m_textures[m_currentCharacterState]);
            }
            m_animator.setAnimation(AnimationType::Move);
        }
    }
    else {
        if (m_timeToIdleAnimation.getElapsedTime().asSeconds() > GameConfig::timeToIdleAnimation) {
            m_currentCharacterState = CharacterState::Idle;
            m_sprite.setTexture(m_textures[CharacterState::Idle]);
            m_animator.setAnimation(AnimationType::Idle);
        }
        else {
            m_currentCharacterState = CharacterState::None;
            m_sprite.setTexture(m_textures[CharacterState::None]);

            int frameWidth = m_textures[CharacterState::None].getSize().x / 4;
            int frameHeight = m_textures[CharacterState::None].getSize().y;
            m_sprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
            m_animator.setAnimation(AnimationType::None);
        }
    }
}

bool Player::checkCollisionWithWalls(const sf::Vector2f& newPosition) {
    std::vector<sf::Sprite> wallSprites = m_world->getCurrentRoom()->getWalls();

    sf::Vector2f rectSize = m_collision.getSize();

    sf::FloatRect futureCollisionRect({
        newPosition.x + rectSize.x,
        newPosition.y + rectSize.y + 12
        }, rectSize);

    for (const auto& wall : wallSprites) {
        if (futureCollisionRect.findIntersection(wall.getGlobalBounds())) {
            return true;
        }
    }

    return false;
}

void Player::onCollision(GameObject* gameObject)
{
    if (Room* room = dynamic_cast<Room*>(gameObject))
    {
        //Door triggers
        auto& doorTriggers = room->getDoorTriggers();
        for (auto& trigger : doorTriggers)
        {
            if (m_collision.getGlobalBounds().findIntersection(trigger.sprite.getGlobalBounds()))
            {
                AdjacentRoomPlace entranceDirection = trigger.direction;

                m_world->moveToRoom(entranceDirection);

                Room* newRoom = m_world->getCurrentRoom();
                if (newRoom)
                {
                    AdjacentRoomPlace exitDirection;
                    switch (entranceDirection)
                    {
                    case AdjacentRoomPlace::left:  exitDirection = AdjacentRoomPlace::right; break;
                    case AdjacentRoomPlace::right: exitDirection = AdjacentRoomPlace::left; break;
                    case AdjacentRoomPlace::top:   exitDirection = AdjacentRoomPlace::bottom; break;
                    case AdjacentRoomPlace::bottom: exitDirection = AdjacentRoomPlace::top; break;
                    }

                    sf::Vector2f newPlayerPos = newRoom->getDoorPosition(exitDirection) - sf::Vector2f{ m_sprite.getGlobalBounds().size * 0.35f } + sf::Vector2f{0, -40};

                    float spriteSize = GameConfig::defaultSpriteSize;

                    switch (exitDirection)
                    {
                    case AdjacentRoomPlace::left:   newPlayerPos.x += spriteSize * 0.9f; break;
                    case AdjacentRoomPlace::right:  newPlayerPos.x -= spriteSize * 1.2f; break;
                    case AdjacentRoomPlace::top:    newPlayerPos.y += spriteSize * 0.9f; newPlayerPos.x += -30; break;
                    case AdjacentRoomPlace::bottom: newPlayerPos.y -= spriteSize * 0.9f; newPlayerPos.x += -30; break;
                    }

                    setPosition(newPlayerPos);
                    if (GameConfig::isDebug)
                    {
                        std::cout << "Trigger activated at: "
                            << trigger.sprite.getPosition().x << ", "
                            << trigger.sprite.getPosition().y << std::endl;
                        std::cout << "Door position in new room: "
                            << newRoom->getDoorPosition(exitDirection).x << ", "
                            << newRoom->getDoorPosition(exitDirection).y << std::endl;
                    }
                }
            }
        }

        //Collide with enemy collision
        auto Monsters = room->getMonsters();
        for (auto monster : Monsters)
        {
            if (m_collision.getGlobalBounds().findIntersection(monster->getCollision()))
            {
                if (!m_isDamageReceived)
                {
                    m_currentHP -= monster->MonsterDamage.bodyDamage;
                    m_isDamageReceived = true;
                    if (GameConfig::isDebug)
                        std::cout << "HP = " << m_currentHP << std::endl;
                }
            }
        }

    }
}

void Player::shoot(const sf::Vector2f& direction) {
    sf::Vector2f bulletPos = getCenterPosition();
    sf::Vector2f normDir = direction;
    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (len != 0.f) normDir /= len;

    m_bullets.push_back(Bullet(bulletPos, normDir, 500.f));
}

void Player::loadTextures()
{
    m_textures[CharacterState::None].loadFromFile("resources/playerCharacters/Cowboy/cowboy_idle_spritesheet.png");
    m_textures[CharacterState::Idle].loadFromFile("resources/playerCharacters/Cowboy/cowboy_idle_spritesheet.png");

    m_textures[CharacterState::MoveUp].loadFromFile("resources/playerCharacters/Cowboy/move/cowboy_walk_up_spritesheet.png");
    m_textures[CharacterState::MoveDown].loadFromFile("resources/playerCharacters/Cowboy/move/cowboy_walk_down_spritesheet.png");
    m_textures[CharacterState::MoveLeft].loadFromFile("resources/playerCharacters/Cowboy/move/cowboy_walk_left_spritesheet.png");
    m_textures[CharacterState::MoveRight].loadFromFile("resources/playerCharacters/Cowboy/move/cowboy_walk_right_spritesheet.png");

    m_textures[CharacterState::AttackUp].loadFromFile("resources/playerCharacters/Cowboy/attack/cowboy_attack_up_spritesheet.png");
    m_textures[CharacterState::AttackDown].loadFromFile("resources/playerCharacters/Cowboy/attack/cowboy_attack_down_spritesheet.png");
    m_textures[CharacterState::AttackLeft].loadFromFile("resources/playerCharacters/Cowboy/attack/cowboy_attack_left_spritesheet.png");
    m_textures[CharacterState::AttackRight].loadFromFile("resources/playerCharacters/Cowboy/attack/cowboy_attack_right_spritesheet.png");
}
