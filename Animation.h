#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

class Animation {
public:
    Animation() = default;
    Animation(sf::Texture* texture, int frameCount, int yIndex, float speed, int frameCountY = 1)
        : m_texture(texture), m_frameCount(frameCount), m_speed(speed)
    {
        int frameWidth = texture->getSize().x / frameCount;
        int frameHeight = texture->getSize().y / frameCountY;

        for (int i = 0; i < frameCount; ++i) {
            m_frames.emplace_back(sf::IntRect({ i * frameWidth, yIndex * frameHeight }, { frameWidth, frameHeight }));
        }
    }

    void update(float dt, sf::Sprite& sprite) {
        m_time += dt;
        if (m_time >= m_speed) {
            m_time = 0;
            m_currentFrame = (m_currentFrame + 1) % m_frameCount;
            sprite.setTextureRect(m_frames[m_currentFrame]);
        }
    }

private:
    sf::Texture* m_texture = nullptr;
    std::vector<sf::IntRect> m_frames;
    int m_frameCount = 0;
    int m_currentFrame = 0;
    float m_speed = 0.1f;
    float m_time = 0;
};