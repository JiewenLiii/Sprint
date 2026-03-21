# Dungeon Adventure Game

A console-based dungeon adventure game implemented in pure C++11.

## Files

```
dungeon_game/
├── main.cpp      # Entry point
├── game.h        # Game class header
├── game.cpp      # Game class implementation
├── map.h         # Map class header
├── map.cpp       # Map class implementation
├── player.h      # Player class header
├── player.cpp    # Player class implementation
├── enemy.h       # Enemy class header
├── enemy.cpp     # Enemy class implementation
└── README.md     # This file
```

## Compile Commands

### Windows (MinGW / MSYS2)
```bash
cd C:\Users\34764\dungeon_game
g++ -std=c++11 -o dungeon.exe main.cpp map.cpp player.cpp enemy.cpp game.cpp
```

### Windows (MSVC - Developer Command Prompt)
```batch
cd C:\Users\34764\dungeon_game
cl /EHsc /std:c++11 main.cpp map.cpp player.cpp enemy.cpp game.cpp
```

### Linux
```bash
cd /path/to/dungeon_game
g++ -std=c++11 -o dungeon main.cpp map.cpp player.cpp enemy.cpp game.cpp
```

### macOS
```bash
cd /path/to/dungeon_game
g++ -std=c++11 -o dungeon main.cpp map.cpp player.cpp enemy.cpp game.cpp
```

## Run

### Windows
```bash
dungeon.exe
```

### Linux / macOS
```bash
./dungeon
```

## Controls

| Key | Action |
|-----|--------|
| W   | Move Up |
| A   | Move Left |
| S   | Move Down |
| D   | Move Right |
| Q   | Quit Game |

## Game Rules

1. **Objective**: Defeat all enemies (3 initially)
2. **Player Stats**: HP 20, Attack 5
3. **Enemy Stats**: HP 10, Attack 3
4. **Combat System**:
   - Combat triggers when player moves onto enemy position
   - Combat triggers when enemy moves onto player position
   - Turn-based combat until one side dies
5. **Enemy AI**:
   - Enemies chase player when within 3 tiles
   - Otherwise patrol randomly

## Legend

| Symbol | Meaning |
|--------|---------|
| @      | Player |
| E      | Enemy |
| #      | Wall (impassable) |
| .      | Floor (walkable) |

## Requirements

- C++11 or higher compiler
- Standard console environment

## Notes

- Game uses `system("cls")` / `system("clear")` for screen clearing
- Random seed is time-based, enemy positions vary each run
