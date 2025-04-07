#include <iostream>
#include "SFML/Graphics.hpp"
#include "GameConfig.h"
#include "GameObject.h"
#include "Player.h"
#include "World.h"
#include "LevelGenerator.h"

#include <thread>

sf::Vector2f PlayerPosition::position = { 0, 0 };

void printCameraPosition(sf::RenderWindow& window, Player& player) {
    static auto lastTime = std::chrono::steady_clock::now();

    using namespace std::chrono;
    auto now = steady_clock::now();
    auto duration = duration_cast<seconds>(now - lastTime);

    if (duration.count() >= 3) {
        sf::View view = window.getView();
        sf::Vector2f cameraPosition = view.getCenter();

        std::cout << "Camera position: ("
            << cameraPosition.x << ", "
            << cameraPosition.y << ")\n";

        std::cout << "Player Position" << player.getPosition().x << ", " << player.getPosition().y << std::endl;
        lastTime = now;
    }
}




void handleWindowEvents(sf::RenderWindow* window)
{
    while (auto event = window->pollEvent())
    {
        if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            window->close();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ GameConfig::screenWidth, GameConfig::screenHeight }), "game");

    Player player(&window);
    //window.setFramerateLimit(60);

    World world(&window, &player);
    world.createLevel();

    window.setView(world.getView());
    

    if (GameConfig::isDebug)
    {
        std::vector<std::vector<int>> level = world.m_levelGenerator.getLevelMap();
        for (const auto& row : level) {
            for (int cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
    }

    
    sf::Clock frameRateClock;


    while (window.isOpen())
    {
        float dt = frameRateClock.restart().asSeconds();
        if (GameConfig::isDebug)
            printCameraPosition(window, player);
        

        handleWindowEvents(&window);

        window.clear(sf::Color::White);
        window.setView(world.getView());

        world.update(dt);
        player.update(dt);


        world.draw();

        player.draw();

        window.display();
    }

    return 0;
}
