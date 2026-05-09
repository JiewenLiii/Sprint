"""
Game Engine - Python implementation of Dungeon Adventure core logic
Ported from C++ version
"""

import random
import math
from typing import List, Optional, Tuple
from dataclasses import dataclass, field
from enum import Enum


class Difficulty(Enum):
    EASY = "easy"
    HARD = "hard"


@dataclass
class DifficultySettings:
    enemy_count: int
    enemy_hp: int
    enemy_attack: int


DIFFICULTY_CONFIG = {
    Difficulty.EASY: DifficultySettings(enemy_count=4, enemy_hp=10, enemy_attack=3),
    Difficulty.HARD: DifficultySettings(enemy_count=8, enemy_hp=15, enemy_attack=6),
}


@dataclass
class Entity:
    """Base entity class for Player and Enemy"""
    x: int = 0
    y: int = 0
    hp: int = 100
    max_hp: int = 100
    attack: int = 10
    id: str = ""
    footprints: List[Tuple[int, int]] = field(default_factory=list)

    def is_alive(self) -> bool:
        return self.hp > 0

    def take_damage(self, damage: int) -> None:
        self.hp = max(0, self.hp - damage)

    def add_footprint(self, x: int, y: int) -> None:
        self.footprints.append((x, y))
        if len(self.footprints) > 5:
            self.footprints.pop(0)


@dataclass
class Player(Entity):
    """Player entity"""
    id: str = "player_001"
    max_hp: int = 20
    hp: int = 20
    attack: int = 5

    def reset(self) -> None:
        self.x = 10
        self.y = 10
        self.hp = self.max_hp
        self.footprints.clear()


@dataclass
class Enemy(Entity):
    """Enemy entity with AI behavior"""
    id: str = "enemy_01"
    color_index: int = 0


class Map:
    """Game map with fog of war and visibility"""

    WIDTH = 20
    HEIGHT = 20
    VIEW_RADIUS = 6

    def __init__(self):
        self.grid = self._generate()
        self.explored = [[False for _ in range(self.WIDTH)] for _ in range(self.HEIGHT)]

    def _generate(self) -> List[List[str]]:
        """Generate a simple dungeon map with walls on edges"""
        grid = []
        for y in range(self.HEIGHT):
            row = []
            for x in range(self.WIDTH):
                if x == 0 or x == self.WIDTH - 1 or y == 0 or y == self.HEIGHT - 1:
                    row.append('#')  # Wall
                else:
                    row.append('.')  # Floor
            grid.append(row)
        return grid

    def is_walkable(self, x: int, y: int) -> bool:
        if 0 <= x < self.WIDTH and 0 <= y < self.HEIGHT:
            return self.grid[y][x] != '#'
        return False

    def is_visible(self, player_x: int, player_y: int, x: int, y: int) -> bool:
        """Check if position (x, y) is visible from player position using Euclidean distance"""
        distance = math.sqrt((x - player_x) ** 2 + (y - player_y) ** 2)
        return distance <= self.VIEW_RADIUS

    def update_explored(self, player_x: int, player_y: int) -> None:
        """Mark visible cells as explored"""
        for y in range(self.HEIGHT):
            for x in range(self.WIDTH):
                if self.is_visible(player_x, player_y, x, y):
                    self.explored[y][x] = True


class Battle:
    """Combat system"""

    MAX_COMBAT_LOG_SIZE = 10

    def execute_combat(
        self,
        player: Player,
        enemy: Enemy,
        enemy_first: bool = False
    ) -> Tuple[bool, List[str]]:
        """
        Execute turn-based combat
        Returns: (player_wins, combat_log)
        """
        combat_log = []

        attacker, defender = (enemy, player) if enemy_first else (player, enemy)
        attacker_name = attacker.id
        defender_name = defender.id

        round_num = 1
        while player.is_alive() and enemy.is_alive():
            damage = self._calculate_damage(attacker.attack)
            defender.take_damage(damage)

            log_entry = f"Round {round_num}: {attacker_name} attacks {defender_name} for {damage} damage"
            combat_log.append(log_entry)

            # Swap attacker and defender
            attacker, defender = defender, attacker
            attacker_name, defender_name = defender_name, attacker_name
            round_num += 1

        # Trim log if too long
        if len(combat_log) > self.MAX_COMBAT_LOG_SIZE:
            combat_log = combat_log[-self.MAX_COMBAT_LOG_SIZE:]

        result = "win" if player.is_alive() else "lose"
        combat_log.append(f"Combat ended: {result}")

        return player.is_alive(), combat_log

    def _calculate_damage(self, attack: int) -> int:
        """Calculate damage with some randomness"""
        return max(1, attack + random.randint(-1, 1))


class GameEngine:
    """Main game engine coordinating all game components"""

    CHASE_RADIUS = 5

    def __init__(self, difficulty: Difficulty = Difficulty.EASY):
        self.difficulty = difficulty
        self.settings = DIFFICULTY_CONFIG[difficulty]
        self.map = Map()
        self.player = Player()
        self.enemies: List[Enemy] = []
        self.game_over = False
        self.victory = False
        self._init_game()

    def _init_game(self) -> None:
        """Initialize game state"""
        self.player.reset()
        self.enemies.clear()
        self._spawn_enemies()
        self.game_over = False
        self.victory = False
        self.map = Map()

    def _spawn_enemies(self) -> None:
        """Spawn enemies at random positions"""
        positions = set()
        positions.add((self.player.x, self.player.y))  # Avoid player position

        colors = [1, 2, 3, 4]  # Different enemy colors

        for i in range(self.settings.enemy_count):
            while True:
                x = random.randint(2, self.map.WIDTH - 3)
                y = random.randint(2, self.map.HEIGHT - 3)
                if (x, y) not in positions:
                    positions.add((x, y))
                    break

            enemy = Enemy(
                id=f"enemy_{i + 1:02d}",
                x=x,
                y=y,
                hp=self.settings.enemy_hp,
                max_hp=self.settings.enemy_hp,
                attack=self.settings.enemy_attack,
                color_index=colors[i % len(colors)]
            )
            self.enemies.append(enemy)

    def restart(self) -> dict:
        """Restart the game"""
        self._init_game()
        return self.get_game_state()

    def get_game_state(self) -> dict:
        """Get current game state for API response"""
        self.map.update_explored(self.player.x, self.player.y)

        return {
            "player": {
                "id": self.player.id,
                "position": [self.player.x, self.player.y],
                "hp": self.player.hp,
                "attack": self.player.attack,
                "colorIndex": 1
            },
            "mapRender": {
                "mapData": self._get_map_data(),
                "fog": self._get_fog_data(),
                "playerPosition": [self.player.x, self.player.y],
                "playerTrail": [list(fp) for fp in self.player.footprints],
                "enemies": self._get_visible_enemies(),
                "enemyTrails": self._get_enemy_trails()
            },
            "message": "游戏开始" if not self.game_over else ("胜利!" if self.victory else "游戏结束")
        }

    def _get_map_data(self) -> List[List[int]]:
        """Convert map to integer array (0=floor, 1=wall)"""
        data = []
        for row in self.map.grid:
            data.append([1 if cell == '#' else 0 for cell in row])
        return data

    def _get_fog_data(self) -> List[List[bool]]:
        """Get fog of war data (True = visible/explored)"""
        fog = []
        for y in range(self.map.HEIGHT):
            row = []
            for x in range(self.map.WIDTH):
                visible = self.map.is_visible(self.player.x, self.player.y, x, y)
                row.append(visible or self.map.explored[y][x])
            fog.append(row)
        return fog

    def _get_visible_enemies(self) -> List[dict]:
        """Get list of visible enemies"""
        visible = []
        for enemy in self.enemies:
            if enemy.is_alive() and self.map.is_visible(
                self.player.x, self.player.y, enemy.x, enemy.y
            ):
                visible.append({
                    "id": enemy.id,
                    "position": [enemy.x, enemy.y],
                    "hp": enemy.hp,
                    "attack": enemy.attack,
                    "colorIndex": enemy.color_index
                })
        return visible

    def _get_enemy_trails(self) -> List[List[List[int]]]:
        """Get enemy footprint trails"""
        trails = []
        for enemy in self.enemies:
            if enemy.is_alive():
                trails.append([list(fp) for fp in enemy.footprints])
        return trails

    def move_player(self, direction: str) -> dict:
        """
        Move player in specified direction
        Returns move result with potential combat trigger
        """
        if self.game_over:
            return {"error": "Game is over", "success": False}

        dx, dy = 0, 0
        if direction == "up":
            dy = -1
        elif direction == "down":
            dy = 1
        elif direction == "left":
            dx = -1
        elif direction == "right":
            dx = 1
        else:
            return {"error": f"Invalid direction: {direction}", "success": False}

        new_x = self.player.x + dx
        new_y = self.player.y + dy

        # Check if move is valid
        if not self.map.is_walkable(new_x, new_y):
            return {"error": "Cannot move there", "success": False}

        # Check for enemy collision
        for enemy in self.enemies:
            if enemy.is_alive() and enemy.x == new_x and enemy.y == new_y:
                return {"error": "Enemy blocking", "success": False}

        # Record footprint before moving
        self.player.add_footprint(self.player.x, self.player.y)

        # Move player
        self.player.x = new_x
        self.player.y = new_y

        # Check for adjacent enemies (combat trigger)
        combat_result = self._check_adjacent_enemies()

        # Update enemies
        self._update_enemies()

        # Check game over
        self._check_game_over()

        result = {
            "success": True,
            "newPosition": [self.player.x, self.player.y],
            "playerStatus": {
                "id": self.player.id,
                "position": [self.player.x, self.player.y],
                "hp": self.player.hp,
                "attack": self.player.attack,
                "isAlive": self.player.is_alive()
            },
            "mapRender": self.get_game_state()["mapRender"]
        }

        if combat_result:
            result["combat"] = combat_result

        return result

    def _check_adjacent_enemies(self) -> Optional[dict]:
        """Check if player is adjacent to any enemy"""
        for enemy in self.enemies:
            if enemy.is_alive():
                dist = abs(self.player.x - enemy.x) + abs(self.player.y - enemy.y)
                if dist == 1:  # Adjacent (not diagonal)
                    return self._start_combat(enemy, enemy_first=False)
        return None

    def _start_combat(self, enemy: Enemy, enemy_first: bool = False) -> dict:
        """Start combat with an enemy"""
        battle = Battle()
        player_wins, combat_log = battle.execute_combat(
            self.player, enemy, enemy_first
        )

        return {
            "enemy": {
                "id": enemy.id,
                "position": [enemy.x, enemy.y],
                "hp": enemy.hp,
                "attack": enemy.attack,
                "colorIndex": enemy.color_index
            },
            "result": "win" if player_wins else "lose",
            "log": combat_log
        }

    def start_combat_api(self) -> dict:
        """API endpoint to start combat with adjacent enemy"""
        for enemy in self.enemies:
            if enemy.is_alive():
                dist = abs(self.player.x - enemy.x) + abs(self.player.y - enemy.y)
                if dist == 1:
                    result = self._start_combat(enemy, enemy_first=False)
                    self._check_game_over()
                    return {
                        "player": {
                            "id": self.player.id,
                            "position": [self.player.x, self.player.y],
                            "hp": self.player.hp,
                            "attack": self.player.attack,
                            "colorIndex": 1
                        },
                        "enemy": result["enemy"],
                        "result": result["result"]
                    }

        return {"error": "No adjacent enemy", "result": None}

    def _update_enemies(self) -> None:
        """Update enemy positions (AI movement)"""
        if not self.player.is_alive():
            return

        for enemy in self.enemies:
            if not enemy.is_alive():
                continue

            dist_x = abs(self.player.x - enemy.x)
            dist_y = abs(self.player.y - enemy.y)

            # Record footprint before moving
            enemy.add_footprint(enemy.x, enemy.y)

            if dist_x <= self.CHASE_RADIUS and dist_y <= self.CHASE_RADIUS:
                self._move_towards_player(enemy)
            else:
                self._random_move(enemy)

            # Check if enemy is now adjacent to player
            new_dist = abs(self.player.x - enemy.x) + abs(self.player.y - enemy.y)
            if new_dist == 1:
                # Enemy initiates combat
                self._start_combat(enemy, enemy_first=True)

    def _move_towards_player(self, enemy: Enemy) -> None:
        """Move enemy towards player"""
        dx = 0 if self.player.x == enemy.x else (1 if self.player.x > enemy.x else -1)
        dy = 0 if self.player.y == enemy.y else (1 if self.player.y > enemy.y else -1)

        # Try horizontal first, then vertical
        new_x, new_y = enemy.x + dx, enemy.y
        if self._can_enemy_move(new_x, new_y, enemy):
            enemy.x = new_x
            return

        new_x, new_y = enemy.x, enemy.y + dy
        if self._can_enemy_move(new_x, new_y, enemy):
            enemy.y = new_y

    def _random_move(self, enemy: Enemy) -> None:
        """Move enemy randomly"""
        directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]
        random.shuffle(directions)

        for dx, dy in directions:
            new_x, new_y = enemy.x + dx, enemy.y + dy
            if self._can_enemy_move(new_x, new_y, enemy):
                enemy.x = new_x
                enemy.y = new_y
                return

    def _can_enemy_move(self, x: int, y: int, current_enemy: Enemy) -> bool:
        """Check if enemy can move to position"""
        if not self.map.is_walkable(x, y):
            return False

        # Check for other enemies
        for enemy in self.enemies:
            if enemy.id != current_enemy.id and enemy.is_alive():
                if enemy.x == x and enemy.y == y:
                    return False

        # Check for player
        if self.player.x == x and self.player.y == y:
            return False

        return True

    def _check_game_over(self) -> None:
        """Check if game is over"""
        if not self.player.is_alive():
            self.game_over = True
            self.victory = False
        elif all(not e.is_alive() for e in self.enemies):
            self.game_over = True
            self.victory = True

    def get_player_status(self) -> dict:
        """Get player status"""
        return {
            "id": self.player.id,
            "position": [self.player.x, self.player.y],
            "hp": self.player.hp,
            "attack": self.player.attack,
            "isAlive": self.player.is_alive()
        }


# Session-based game instance management
# Each session ID maps to a separate GameEngine instance
import uuid

_game_sessions: dict[str, GameEngine] = {}


def create_session() -> tuple[str, GameEngine]:
    """Create a new game session and return (session_id, game)"""
    session_id = str(uuid.uuid4())[:8]
    game = GameEngine()
    _game_sessions[session_id] = game
    return session_id, game


def get_game(session_id: str) -> Optional[GameEngine]:
    """Get game instance by session ID"""
    return _game_sessions.get(session_id)


def reset_game(session_id: str, difficulty: Difficulty = Difficulty.EASY) -> GameEngine:
    """Reset game for a specific session"""
    game = GameEngine(difficulty)
    _game_sessions[session_id] = game
    return game
