#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <filesystem>
#include <iostream>

// The sprite assets are third-party and are not part of the repository (see README).
// When a file is missing, a coloured placeholder of the same size is generated instead,
// so the game still starts and is playable without any manual setup.

struct TexturePlaceholder {
    sf::Vector2u size;       // same size as the real sprite sheet, so frame calculations stay valid
    sf::Color color;
    unsigned columns = 1;    // frames or tiles across; each one gets a slightly different shade
    unsigned rows = 1;
};

namespace Placeholders {
    inline const TexturePlaceholder tileset{ { 160, 160 }, { 95, 85, 120 }, 10, 10 };
    inline const TexturePlaceholder corner{ { 16, 16 }, { 70, 60, 90 } };

    inline const TexturePlaceholder cowboyIdle{ { 128, 32 }, { 170, 110, 50 }, 4, 1 };
    inline const TexturePlaceholder cowboyWalk{ { 128, 32 }, { 170, 110, 50 }, 4, 1 };
    inline const TexturePlaceholder cowboyAttack{ { 160, 32 }, { 200, 140, 60 }, 5, 1 };

    inline const TexturePlaceholder slimeIdle{ { 384, 256 }, { 70, 170, 90 }, 6, 4 };
    inline const TexturePlaceholder slimeWalk{ { 512, 256 }, { 70, 170, 90 }, 8, 4 };
    inline const TexturePlaceholder slimeRun{ { 512, 256 }, { 90, 190, 110 }, 8, 4 };
    inline const TexturePlaceholder slimeAttack{ { 640, 256 }, { 60, 150, 80 }, 10, 4 };
}

inline sf::Texture loadTexture(const std::filesystem::path& path, const TexturePlaceholder& placeholder)
{
    sf::Texture texture;
    if (texture.loadFromFile(path))
    {
        return texture;
    }

    static bool reported = false;
    if (!reported)
    {
        std::cerr << "[assets] \"" << path.string() << "\" not found, using placeholders. "
                     "See the README to add the sprite packs.\n";
        reported = true;
    }

    sf::Image image(placeholder.size, placeholder.color);
    const unsigned cellWidth = std::max(1u, placeholder.size.x / placeholder.columns);
    const unsigned cellHeight = std::max(1u, placeholder.size.y / placeholder.rows);

    for (unsigned y = 0; y < placeholder.size.y; ++y)
    {
        for (unsigned x = 0; x < placeholder.size.x; ++x)
        {
            const unsigned shade = (((x / cellWidth) * 3 + (y / cellHeight) * 5) % 4) * 18;
            image.setPixel({ x, y }, sf::Color(
                static_cast<std::uint8_t>(std::min(255u, placeholder.color.r + shade)),
                static_cast<std::uint8_t>(std::min(255u, placeholder.color.g + shade)),
                static_cast<std::uint8_t>(std::min(255u, placeholder.color.b + shade))));
        }
    }

    (void)texture.loadFromImage(image);
    return texture;
}
