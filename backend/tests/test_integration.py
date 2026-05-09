"""
API Integration Tests for Dungeon Adventure Game
Tests the full API flow: game start, player moves, combat, etc.
"""

import pytest
import requests
import time

BASE_URL = "http://localhost:8080"


@pytest.fixture(scope="module")
def base_url():
    """Base URL for API tests"""
    return BASE_URL


@pytest.fixture(autouse=True)
def reset_game_state(base_url):
    """Reset game state before each test"""
    requests.post(f"{base_url}/game/restart")
    yield


class TestHealth:
    """Health check tests"""

    def test_health_endpoint(self, base_url):
        """Test health check endpoint"""
        response = requests.get(f"{base_url}/health")
        assert response.status_code == 200
        data = response.json()
        assert data["status"] == "healthy"
        assert data["service"] == "dungeon-adventure-api"


class TestGameStart:
    """Game initialization tests"""

    def test_start_game(self, base_url):
        """Test starting a new game"""
        response = requests.post(f"{base_url}/game/start")
        assert response.status_code == 200
        data = response.json()

        # Check player data
        assert "player" in data
        assert data["player"]["id"] == "player_001"
        assert data["player"]["attack"] == 5
        assert len(data["player"]["position"]) == 2

        # Check map render data
        assert "mapRender" in data
        assert "mapData" in data["mapRender"]
        assert "enemies" in data["mapRender"]
        assert len(data["mapRender"]["enemies"]) > 0

    def test_restart_game(self, base_url):
        """Test restarting the game"""
        response = requests.post(f"{base_url}/game/restart")
        assert response.status_code == 200
        data = response.json()

        assert "player" in data
        assert "mapRender" in data
        assert data["message"] == "游戏重新开始"


class TestPlayerMovement:
    """Player movement tests"""

    @pytest.fixture
    def game_state(self, base_url):
        """Get fresh game state"""
        requests.post(f"{base_url}/game/restart")
        return True

    def test_move_up(self, base_url, game_state):
        """Test moving player up"""
        # Get initial position
        response = requests.get(f"{base_url}/game/player/status")
        initial_pos = response.json()["position"]

        # Move up
        response = requests.post(
            f"{base_url}/game/player/move",
            json={"direction": "up"}
        )
        assert response.status_code == 200
        data = response.json()

        assert "newPosition" in data
        assert data["newPosition"][1] == initial_pos[1] - 1
        assert data["newPosition"][0] == initial_pos[0]

    def test_move_down(self, base_url, game_state):
        """Test moving player down"""
        response = requests.post(
            f"{base_url}/game/player/move",
            json={"direction": "down"}
        )
        assert response.status_code == 200
        data = response.json()
        assert "newPosition" in data

    def test_move_left(self, base_url, game_state):
        """Test moving player left"""
        response = requests.post(
            f"{base_url}/game/player/move",
            json={"direction": "left"}
        )
        assert response.status_code == 200
        data = response.json()
        assert "newPosition" in data

    def test_move_right(self, base_url, game_state):
        """Test moving player right"""
        response = requests.post(
            f"{base_url}/game/player/move",
            json={"direction": "right"}
        )
        assert response.status_code == 200
        data = response.json()
        assert "newPosition" in data

    def test_invalid_direction(self, base_url, game_state):
        """Test invalid direction returns error"""
        response = requests.post(
            f"{base_url}/game/player/move",
            json={"direction": "invalid"}
        )
        assert response.status_code == 422  # Validation error


class TestPlayerStatus:
    """Player status tests"""

    def test_get_player_status(self, base_url):
        """Test getting player status"""
        response = requests.get(f"{base_url}/game/player/status")
        assert response.status_code == 200
        data = response.json()

        assert "id" in data
        assert "position" in data
        assert "hp" in data
        assert "attack" in data
        assert "isAlive" in data
        assert isinstance(data["isAlive"], bool)


class TestMapRender:
    """Map rendering tests"""

    def test_get_map_render(self, base_url):
        """Test getting map render data"""
        response = requests.get(f"{base_url}/game/map/render")
        assert response.status_code == 200
        data = response.json()

        assert "mapData" in data
        assert "fog" in data
        assert "playerPosition" in data
        assert "enemies" in data
        assert isinstance(data["mapData"], list)
        assert len(data["mapData"]) == 20  # 20x20 map


class TestCombat:
    """Combat system tests"""

    @pytest.fixture
    def game_state(self, base_url):
        """Get fresh game state"""
        requests.post(f"{base_url}/game/restart")
        return True

    def test_combat_no_enemy(self, base_url, game_state):
        """Test combat start when no enemy adjacent"""
        response = requests.post(f"{base_url}/game/combat/start")
        # Should return error since no enemy is adjacent
        assert response.status_code == 400


class TestGameFlow:
    """End-to-end game flow tests"""

    def test_full_game_flow(self, base_url):
        """Test complete game flow"""
        # 1. Start game
        response = requests.post(f"{base_url}/game/start")
        assert response.status_code == 200

        # 2. Move around
        directions = ["up", "down", "left", "right"]
        for direction in directions:
            response = requests.post(
                f"{base_url}/game/player/move",
                json={"direction": direction}
            )
            # Movement might fail due to walls or enemies, but should not crash
            assert response.status_code in [200, 400]

        # 3. Get status
        response = requests.get(f"{base_url}/game/player/status")
        assert response.status_code == 200

        # 4. Get map
        response = requests.get(f"{base_url}/game/map/render")
        assert response.status_code == 200

        # 5. Restart
        response = requests.post(f"{base_url}/game/restart")
        assert response.status_code == 200


class TestCORS:
    """CORS configuration tests"""

    def test_cors_headers(self, base_url):
        """Test CORS headers are present"""
        response = requests.options(
            f"{base_url}/game/start",
            headers={
                "Origin": "http://localhost:5173",
                "Access-Control-Request-Method": "POST"
            }
        )
        assert "Access-Control-Allow-Origin" in response.headers


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
