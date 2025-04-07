#pragma once
#include <map>
#include "Animation.h"

enum class AnimationType
{
    None,
    Idle,
    Move,
    MoveDown,
    MoveLeft,
    MoveRight,
    MoveUp,
    Dash,
    DashDown,
    DashLeft,
    DashRight,
    DashUp,
    Attack,
};

class Animator {
public:
    void addAnimation(AnimationType dir, Animation animation) {
        m_animations[dir] = animation;
    }

    void setAnimation(AnimationType dir) {
        if (m_currentAnimation != &m_animations[dir]) {
            m_currentAnimation = &m_animations[dir];
        }
    }

    void update(float dt, sf::Sprite& sprite) {
        if (m_currentAnimation)
            m_currentAnimation->update(dt, sprite);
    }

private:
    std::map<AnimationType, Animation> m_animations;
    Animation* m_currentAnimation = nullptr;
};
