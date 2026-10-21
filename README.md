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

C++17, SFML (graphics), CMake.

## Project layout

| Path | Purpose |
|---|---|
| `game.cpp` | Entry point and main loop |
| `World.*`, `Room*.*`, `Level*.*` | World, rooms and level generation |
| `Player.*`, `Monster.*`, `Bullet.h` | Game entities |
| `Animation.h`, `Animator.h` | Sprite-sheet animations |
| `Collider.*`, `DamageManager.h` | Collisions and damage |
| `docs/` | Notes on the architecture (in Ukrainian) and the demo video |

## Known limitations

- Sprite assets (`resources/`) are not included in this repository yet, so the game cannot be run from a fresh clone.
- `CMakeLists.txt` is outdated: it targets SFML 2.5 and builds only `game.cpp`, so it needs to be updated for the SFML 3 API used in the code.
- Shooting and the monster area attack are only partially implemented.
