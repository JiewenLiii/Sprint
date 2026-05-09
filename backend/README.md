# Dungeon Adventure Backend API

FastAPI-based backend for the Dungeon Adventure game.

## Setup

```bash
pip install -r requirements.txt
uvicorn main:app --reload --host 0.0.0.0 --port 8080
```

## API Endpoints

- `POST /game/start` - Initialize a new game
- `POST /game/player/move` - Move player
- `POST /game/combat/start` - Start combat
- `GET /game/map/render` - Get map data
- `GET /game/player/status` - Get player status
- `POST /game/restart` - Restart game
