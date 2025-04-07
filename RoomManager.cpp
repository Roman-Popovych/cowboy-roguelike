//#include "RoomManager.h"
//#include "RoomGenerator.h"
//
//RoomManager::RoomManager(sf::RenderWindow& window)
//    : m_window(window)
//    , m_roomGenerator()
//{
//    //generateNewLevel();
//}
//
////void RoomManager::generateNewLevel()
////{
////    m_rooms.clear();
////    m_currentRoomIndex = {0, 0};
////    
////    // Створюємо початкову кімнату
////    createRoom(0, 0);
////}
//
//void RoomManager::createRoom(int widthInSprites, int heightInSprites, float globalX, float globalY, int mapPosX, int mapPosY)
//{
//    //Room* room = m_roomGenerator.generateRoom(widthInSprites, heightInSprites, globalX, globalY); // Фіксований розмір кімнати
//    //m_rooms.emplace(std::make_pair(mapPosX, mapPosY), room);
//}
//
////void RoomManager::moveToRoom(int dx, int dy)
////{
////    if (isValidMove(dx, dy))
////    {
////        m_currentRoomIndex.first += dx;
////        m_currentRoomIndex.second += dy;
////        
////        // Створюємо нову кімнату, якщо вона ще не існує
////        if (m_rooms.find(m_currentRoomIndex) == m_rooms.end())
////        {
////            createRoom(m_currentRoomIndex.first, m_currentRoomIndex.second);
////        }
////    }
////}
//
//bool RoomManager::isValidMove(int dx, int dy) const
//{
//    // Тут можна додати додаткову логіку перевірки можливості руху
//    return true;
//}
//
////void RoomManager::handleInput(const sf::Event& event) {
////    if (event.is<sf::Event::KeyPressed>()) {
////        const auto& keyEvent = event.getIf<sf::Event::KeyPressed>();
////        switch (keyEvent->code) {
////        case sf::Keyboard::Key::J:
////            moveToRoom(0, -1);
////            break;
////        case sf::Keyboard::Key::L:
////            moveToRoom(0, 1);
////            break;
////        case sf::Keyboard::Key::I:
////            moveToRoom(-1, 0);
////            break;
////        case sf::Keyboard::Key::K:
////            moveToRoom(1, 0);
////            break;
////        }
////    }
////}
//
////void RoomManager::update(float deltaTime)
////{
////    auto it = m_rooms.find(m_currentRoomIndex);
////    if (it != m_rooms.end())
////    {
////        it->second.update(deltaTime);
////    }
////}
////
////void RoomManager::draw()
////{
////    auto it = m_rooms.find(m_currentRoomIndex);
////    if (it != m_rooms.end())
////    {
////        it->second.draw(m_window);
////    }
////} 
//
//void RoomManager::update(float deltaTime)
//{
//    for (auto& [index, room] : m_rooms)
//    {
//        room.update(deltaTime);
//    }
//}
//
//void RoomManager::draw()
//{
//    for (auto& [index, room] : m_rooms)
//    {
//        room.draw(m_window);
//    }
//}