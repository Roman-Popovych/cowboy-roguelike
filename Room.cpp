//#include "Room.h"
//
//Room::Room(sf::RenderWindow* window, int id)
//    : m_window(window)
//    , m_id(id)
//{
//    m_roomShape.setSize(sf::Vector2f(1200, 900));
//    m_roomShape.setFillColor(sf::Color(100, 100, 100));
//    
//    // Відцентровуємо кімнату
//    sf::Vector2u windowSize = m_window->getSize();
//    float x = (windowSize.x - m_roomShape.getSize().x) / 2.0f;
//    float y = (windowSize.y - m_roomShape.getSize().y) / 2.0f;
//    m_roomShape.setPosition({ x, y });
//}
//
//void Room::update(float dt)
//{
//    for (auto& gameObject : m_gameObjects)
//    {
//        gameObject->update(dt);
//    }
//}
//
//void Room::draw()
//{
//    m_window->draw(m_roomShape);
//    
//    // Малюємо частини кімнати
//    for (const auto& part : m_roomParts)
//    {
//        sf::Sprite sprite = part.sprite;
//        sprite.setPosition(part.position);
//        m_window->draw(sprite);
//    }
//    
//    // Малюємо об'єкти кімнати
//    for (auto& gameObject : m_gameObjects)
//    {
//        gameObject->draw();
//    }
//}
//
//void Room::addGameObject(GameObject* gameObject)
//{
//    m_gameObjects.push_back(gameObject);
//}
//
//void Room::removeGameObject(GameObject* gameObject)
//{
//    auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
//    if (it != m_gameObjects.end())
//    {
//        m_gameObjects.erase(it);
//    }
//}
//
//void Room::addRoomPart(const sf::Sprite& sprite, const sf::Vector2f& position, const std::string& type)
//{
//    RoomPart part;
//    part.sprite = sprite;
//    part.position = position;
//    part.type = type;
//    m_roomParts.push_back(part);
//}
//
//void Room::clearRoomParts()
//{
//    m_roomParts.clear();
//}
