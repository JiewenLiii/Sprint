"""
Dungeon Adventure - FastAPI Backend
RESTful API for the web-based dungeon adventure game
"""

from fastapi import FastAPI, HTTPException, Header, Body
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from typing import Optional, List
from enum import Enum

from game_engine import create_session, get_game, reset_game, Difficulty, GameEngine


# Pydantic models for request/response
class DirectionEnum(str, Enum):
    up = "up"
    down = "down"
    left = "left"
    right = "right"


class DifficultyEnum(str, Enum):
    easy = "easy"
    hard = "hard"


class MoveRequest(BaseModel):
    direction: DirectionEnum


class Player(BaseModel):
    id: str
    position: List[int]
    hp: int
    attack: int
    maxHp: Optional[int] = 20
    colorIndex: Optional[int] = 1
    isAlive: Optional[bool] = True


class Enemy(BaseModel):
    id: str
    position: List[int]
    hp: int
    attack: int
    colorIndex: int


class MapRender(BaseModel):
    mapData: Optional[List[List[int]]] = None
    fog: Optional[List[List[bool]]] = None
    playerPosition: List[int]
    playerTrail: Optional[List[List[int]]] = None
    enemies: List[dict]
    enemyTrails: Optional[List[List[List[int]]]] = None


class GameInitResponse(BaseModel):
    sessionId: str
    mapRender: MapRender
    player: Player
    aliveEnemiesCount: int
    message: str


class PlayerStatusResponse(BaseModel):
    id: str
    position: List[int]
    hp: int
    attack: int
    maxHp: Optional[int] = 20
    isAlive: bool


class MoveResponse(BaseModel):
    newPosition: List[int]
    playerStatus: PlayerStatusResponse
    mapRender: MapRender
    aliveEnemiesCount: int
    combat: Optional[dict] = None


class CombatResult(BaseModel):
    player: Player
    enemy: Enemy
    result: str


# Default session ID for backward compatibility (single-user)
DEFAULT_SESSION = "default"


def _resolve_session(session_id: Optional[str] = None) -> str:
    """Resolve session ID from header or use default"""
    return session_id or DEFAULT_SESSION


def _get_or_create_game(session_id: str) -> GameEngine:
    """Get existing game or create a new session"""
    game = get_game(session_id)
    if game is None:
        session_id, game = create_session()
    return game


# FastAPI app
app = FastAPI(
    title="Dungeon Adventure Game API",
    description="地牢探险游戏全量接口契约",
    version="2.2.0"
)

# CORS middleware - allow frontend access
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # In production, specify exact origins
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.post("/game/start", response_model=GameInitResponse, tags=["Game"])
async def game_start(
    x_session_id: Optional[str] = Header(None),
    difficulty: str = Body("easy", embed=True)
):
    """Initialize a new game. Difficulty: 'easy' or 'hard'."""
    try:
        diff = Difficulty(difficulty)
    except ValueError:
        diff = Difficulty.EASY
    session_id, game = create_session(diff)

    state = game.get_game_state()

    return GameInitResponse(
        sessionId=session_id,
        mapRender=MapRender(**state["mapRender"]),
        player=Player(**state["player"]),
        aliveEnemiesCount=state["aliveEnemiesCount"],
        message=state["message"]
    )


@app.post("/game/player/move", response_model=MoveResponse, tags=["Player"])
async def player_move(
    request: MoveRequest,
    x_session_id: Optional[str] = Header(None)
):
    """Move player in specified direction"""
    session_id = _resolve_session(x_session_id)
    game = _get_or_create_game(session_id)
    result = game.move_player(request.direction.value)

    if not result.get("success"):
        raise HTTPException(status_code=400, detail=result.get("error", "Move failed"))

    return MoveResponse(
        newPosition=result["newPosition"],
        playerStatus=PlayerStatusResponse(**result["playerStatus"]),
        mapRender=MapRender(**result["mapRender"]),
        aliveEnemiesCount=result["aliveEnemiesCount"],
        combat=result.get("combat")
    )


@app.post("/game/combat/start", response_model=CombatResult, tags=["Combat"])
async def combat_start(x_session_id: Optional[str] = Header(None)):
    """Start combat with adjacent enemy"""
    session_id = _resolve_session(x_session_id)
    game = _get_or_create_game(session_id)
    result = game.start_combat_api()

    if result.get("error"):
        raise HTTPException(status_code=400, detail=result["error"])

    return CombatResult(
        player=Player(**result["player"]),
        enemy=Enemy(**result["enemy"]),
        result=result["result"]
    )


@app.get("/game/map/render", tags=["Map"])
async def map_render(x_session_id: Optional[str] = Header(None)):
    """Get current map render data"""
    session_id = _resolve_session(x_session_id)
    game = _get_or_create_game(session_id)
    state = game.get_game_state()
    return state["mapRender"]


@app.get("/game/player/status", response_model=PlayerStatusResponse, tags=["Player"])
async def player_status(x_session_id: Optional[str] = Header(None)):
    """Get player current status"""
    session_id = _resolve_session(x_session_id)
    game = _get_or_create_game(session_id)
    status = game.get_player_status()
    return PlayerStatusResponse(**status)


@app.post("/game/restart", response_model=GameInitResponse, tags=["Game"])
async def game_restart(
    x_session_id: Optional[str] = Header(None),
    difficulty: str = Body("easy", embed=True)
):
    """Restart the game"""
    session_id = _resolve_session(x_session_id)
    try:
        diff = Difficulty(difficulty)
    except ValueError:
        diff = Difficulty.EASY
    game = reset_game(session_id, diff)
    state = game.get_game_state()

    return GameInitResponse(
        sessionId=session_id,
        mapRender=MapRender(**state["mapRender"]),
        player=Player(**state["player"]),
        aliveEnemiesCount=state["aliveEnemiesCount"],
        message="游戏重新开始"
    )


@app.get("/health", tags=["System"])
async def health_check():
    """Health check endpoint"""
    return {"status": "healthy", "service": "dungeon-adventure-api"}


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8080)
