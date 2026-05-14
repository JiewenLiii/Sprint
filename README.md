# Dungeon Adventure - 地牢探险

基于 **Python FastAPI + Vue 3** 的 Web 地牢探险游戏，采用前后端分离架构，支持 Docker 一键部署。

## 功能特性

- **20×20 地牢地图** - 圆形视野系统 + 战争迷雾
- **回合制战斗** - 战斗弹窗动画，逐回合展示攻击日志
- **敌人 AI** - 追击（6 格内）/ 巡逻（6 格外），4 方向移动
- **双难度模式** - Easy（4 敌人）/ Hard（8 敌人）
- **足迹系统** - 玩家/敌人足迹渐变显示
- **多会话隔离** - 通过 Session ID 支持多用户同时游戏

## 系统架构

```
┌─────────────────┐         RESTful API         ┌──────────────────┐
│   Vue 3 前端     │  ←─── X-Session-Id ────→   │  FastAPI 后端     │
│   (Nginx:80)    │         JSON                │  (Uvicorn:8080)  │
│                 │                              │                  │
│  App.vue        │   POST /game/start           │  main.py         │
│  GameMap.vue    │   POST /game/player/move     │  game_engine.py  │
│  CombatModal.vue│   POST /game/combat/start    │                  │
│  GameLog.vue    │   GET  /game/map/render      │  地图生成         │
│  PlayerStatus   │   GET  /game/player/status   │  战斗结算         │
│  DifficultyModal│   POST /game/restart         │  敌人 AI         │
└─────────────────┘                              └──────────────────┘
         │                                              │
         └──────────── docker-compose ──────────────────┘
```

## 本地开发

### 前置要求

- Python 3.11+
- Node.js 18+
- Docker & Docker Compose（容器化部署）

### 方式一：本地启动

```bash
# 后端
cd backend
pip install -r requirements.txt
python main.py
# → http://localhost:8080

# 前端（新终端）
cd frontend
npm install
npm run dev
# → http://localhost:5173
```

### 方式二：Docker 一键启动

```bash
docker-compose up --build
# → http://localhost
```

### 方式三：Windows 一键启动

```bash
start.bat
```

## 项目结构

```
DungeonAdventure/
├── backend/
│   ├── main.py              # FastAPI 入口（7 个 RESTful 端点）
│   ├── game_engine.py       # 游戏引擎（地图、战斗、敌人 AI、会话管理）
│   ├── requirements.txt     # 运行依赖
│   ├── requirements-test.txt # 测试依赖
│   ├── Dockerfile
│   └── tests/
│       └── test_integration.py  # API 集成测试（14 个）
├── frontend/
│   ├── index.html
│   ├── package.json
│   ├── vite.config.js       # Vite 配置（API 代理）
│   ├── Dockerfile           # 多阶段构建（Node + Nginx）
│   └── src/
│       ├── main.js
│       ├── App.vue          # 主组件
│       ├── api/
│       │   └── gameApi.js   # API 客户端
│       └── components/
│           ├── GameMap.vue
│           ├── GameLog.vue
│           ├── PlayerStatus.vue
│           ├── CombatModal.vue
│           ├── DifficultyModal.vue
│           └── ControlPanel.vue
├── docker-compose.yml
├── .github/workflows/
│   └── docker-build.yml     # CI/CD（集成测试 + GHCR 镜像构建）
├── docs/
│   ├── openapi/v2/dungeon-adventure.yaml  # OpenAPI 接口契约
│   └── ...
├── AGENTS.md
└── README.md
```

## API 端点

| 方法 | 路径 | 说明 |
|------|------|------|
| POST | `/game/start` | 开始新游戏（参数：difficulty） |
| POST | `/game/restart` | 重新开始 |
| POST | `/game/player/move` | 玩家移动（参数：direction） |
| POST | `/game/combat/start` | 触发战斗 |
| GET | `/game/map/render` | 获取地图渲染数据 |
| GET | `/game/player/status` | 获取玩家状态 |
| GET | `/health` | 健康检查 |

详细接口文档：`docs/openapi/v2/dungeon-adventure.yaml`

## 测试

```bash
cd backend
pip install -r requirements-test.txt
pytest tests/test_integration.py -v
```

## 技术栈

| 层 | 技术 |
|----|------|
| 后端 | Python 3.11, FastAPI, Pydantic, Uvicorn |
| 前端 | Vue 3 (Composition API), Bootstrap 5, Vite |
| 容器 | Docker, Docker Compose, Nginx |
| CI/CD | GitHub Actions, GHCR |
| 测试 | pytest, httpx |

## 许可证

本项目仅供学习用途。
