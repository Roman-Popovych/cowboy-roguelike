# Cowboy Roguelike (working title)

A top-down, room-based roguelike prototype written in C++17 with SFML, inspired by *The Binding of Isaac*. The player is a cowboy who moves between procedurally generated rooms, shoots and fights monsters.

> **Status:** early prototype, a learning project. Not all planned features are implemented.

[Gameplay video](docs/demo.mp4)

## What is implemented

- **Procedural levels:** a 2D map of rooms is generated and assembled from sprites (`LevelGenerator`, `RoomGenerator`, `LevelConstructor`).
- **Room navigation:** moving between rooms through door triggers.
- **Player:** walking and shooting animations, health bar, collision handling, damage cooldown.
- **Monsters:** wandering, a dash attack towards the player when in range, contact damage.
- **Systems:** `Collider` (overlap detection), `DamageManager` (damage handling), `World` (owns the main objects and connects them).

## Tech

C++20, SFML 3.0.0, Visual Studio 2022 solution (`game.sln`).

## Project layout

| Path | Purpose |
|---|---|
| `game.cpp` | Entry point and main loop |
| `World.*`, `Room*.*`, `Level*.*` | World, rooms and level generation |
| `Player.*`, `Monster.*`, `Bullet.h` | Game entities |
| `Animation.h`, `Animator.h` | Sprite-sheet animations |
| `Collider.*`, `DamageManager.h` | Collisions and damage |
| `docs/` | Notes on the architecture (in Ukrainian) and the demo video |

## Build and run (Windows)

1. Install Visual Studio 2022 with the "Desktop development with C++" workload.
2. Download [SFML 3.0.0 for Visual C++ 17 (64-bit)](https://www.sfml-dev.org/download/sfml/3.0.0/) and unpack it to `C:\GameDev\Course\SFML-3.0.0-windows-vc17-64-bit\SFML-3.0.0`. This is the path stored in the project; to use another location, change the include and library directories in the project properties.
3. Open `game.sln`, select the `x64` platform and build.
4. Copy the SFML DLLs from `SFML-3.0.0\bin` next to the built `game.exe` (or add that folder to `PATH`).
5. Run from Visual Studio, or from the folder that contains `resources/`, because assets are loaded by relative path.

## Known limitations

- Shooting and the monster area attack are only partially implemented.
- `CMakeLists.txt` is a leftover: it targets SFML 2.5 and builds only `game.cpp`. Use the Visual Studio solution.
- Asset paths differ in letter case from the folder names on disk (for example `playerCharacters` vs `PlayerCharacters`). This works on Windows, but would break on case-sensitive systems.

## Credits

The sprites come from third-party free asset packs and keep their original licenses:

- 2D Pixel Dungeon Asset Pack
- Free Slime Mobs Pixel Art Top-Down Sprite Pack (Craftpix)
- Wild West Pixel Cowboy - Pixel Art Asset Pack
