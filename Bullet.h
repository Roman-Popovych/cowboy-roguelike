#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(const sf::Vector2f& position, const sf::Vector2f& direction, float speed)
        : m_direction(direction), m_speed(speed)
    {
        m_shape.setRadius(5.f);
        m_shape.setFillColor(sf::Color::Yellow);
        m_shape.setPosition(position);
    }

    void update(float dt) {
        m_shape.move(m_direction * m_speed * dt);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(m_shape);
    }

    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_direction;
    float m_speed;
};