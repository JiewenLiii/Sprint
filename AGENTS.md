# Dungeon Adventure - AGENTS.md

## 一、项目架构概述

本文档遵循 **Anthropic 渐进式披露范式**，供 AI 编程助手快速理解项目全貌。

本项目为基于 **Python FastAPI + Vue 3** 的 Web 地牢探险游戏，采用前后端分离架构：
- **后端**：Python FastAPI RESTful API，承载全部游戏逻辑（地图生成、战斗结算、敌人 AI）
- **前端**：Vue 3 Composition API + Bootstrap 5，提供 2D 地图界面、战斗弹窗、玩家状态面板
- **容器化**：Docker + docker-compose 编排，支持一键部署
- **CI/CD**：GitHub Actions 自动运行集成测试，构建镜像推送至 GHCR

核心业务：双难度选择（Easy/Hard）、20×20 地牢地图探索、圆形视野迷雾系统、回合制战斗、敌人 AI 追击/巡逻。

## 二、目录结构

```
DungeonAdventure/
├── backend/
│   ├── main.py              # FastAPI 入口，7 个 RESTful 端点
│   ├── game_engine.py       # 游戏引擎（地图、战斗、敌人 AI、会话管理）
│   ├── requirements.txt     # 运行依赖
│   ├── requirements-test.txt # 测试依赖
│   ├── Dockerfile
│   └── tests/
│       └── test_integration.py  # 14 个 API 集成测试
├── frontend/
│   ├── index.html
│   ├── package.json
│   ├── vite.config.js
│   ├── Dockerfile
│   └── src/
│       ├── main.js
│       ├── App.vue          # 主组件（状态管理、键盘控制、战斗逻辑）
│       ├── api/
│       │   └── gameApi.js   # API 客户端（Session 管理）
│       └── components/
│           ├── GameMap.vue      # 地图渲染（迷雾、足迹渐变）
│           ├── GameLog.vue      # 日志面板（滚动 + 重启按钮）
│           ├── PlayerStatus.vue # 玩家状态卡片
│           ├── CombatModal.vue  # 战斗弹窗（动画 + 逐回合展示）
│           ├── DifficultyModal.vue # 难度选择弹窗
│           └── ControlPanel.vue # 操作提示（WASD）
├── docker-compose.yml
├── .github/workflows/
│   └── docker-build.yml     # CI：集成测试 + Docker 镜像构建（GHCR）
├── docs/
│   ├── openapi/v2/dungeon-adventure.yaml  # OpenAPI 接口契约
│   └── ...                  # 各阶段交付文档
├── AGENTS.md
├── README.md
└── start.bat                # Windows 一键启动脚本
```

## 三、核心模块职责

### 后端 (Python)

| 文件 | 职责 |
|------|------|
| `main.py` | FastAPI 应用，定义 7 个端点：`/game/start`, `/game/restart`, `/game/player/move`, `/game/combat/start`, `/game/map/render`, `/game/player/status`, `/health`。通过 `X-Session-Id` Header 实现多用户会话隔离。 |
| `game_engine.py` | `GameEngine` 类：地图生成（随机墙壁 + DFS 可达性验证）、圆形视野计算、敌人 AI（追击/巡逻）、回合制战斗、足迹系统。`session_manager` 全局字典管理多会话。 |

### 前端 (Vue 3)

| 文件 | 职责 |
|------|------|
| `App.vue` | 状态中枢：管理玩家、地图、敌人、日志状态。监听键盘 WASD/方向键，调用 API，分发数据给子组件。 |
| `GameMap.vue` | 20×20 网格渲染：墙壁/地板/玩家/敌人/足迹，迷雾遮罩，足迹颜色渐变。 |
| `CombatModal.vue` | 战斗弹窗：逐回合动画展示攻击日志，实时更新 HP 血条。 |
| `gameApi.js` | 封装 fetch，自动携带 `X-Session-Id`，统一错误处理。 |

## 四、编码规范

1. **后端**：Python 3.11，遵循 PEP 8，使用 Pydantic 模型校验请求/响应
2. **前端**：Vue 3 `<script setup>` + Composition API，不使用 Options API
3. **样式**：纯 CSS + Bootstrap 5 工具类，不使用 CSS 预处理器
4. **API 协议**：RESTful，请求体 JSON，`Content-Type: application/json`
5. **测试**：pytest + httpx，集成测试覆盖全部 7 个端点
6. **提交信息**：`type(scope): description` 格式（如 `feat(frontend): 战斗弹窗`）

## 五、禁止操作清单

1. 禁止在前端直接修改游戏状态，所有状态变更必须通过后端 API
2. 禁止删除或修改现有集成测试用例
3. 禁止在 `game_engine.py` 中引入前端相关依赖
4. 禁止硬编码数值（地图尺寸、视野半径、初始属性等必须用常量）
5. 禁止绕过 `X-Session-Id` 直接操作全局游戏状态
6. 禁止在 `main.py` 中编写游戏业务逻辑（业务逻辑属于 `game_engine.py`）

## 六、本地开发

```bash
# 后端
cd backend && pip install -r requirements.txt && python main.py

# 前端
cd frontend && npm install && npm run dev

# Docker 一键启动
docker-compose up --build
```

## 七、AI 辅助开发指引

- 新增 API 端点：在 `main.py` 添加路由，在 `game_engine.py` 添加业务逻辑，在 `test_integration.py` 添加测试
- 新增前端组件：在 `src/components/` 创建 Vue 文件，在 `App.vue` 中引入并传入 props
- 修改游戏规则：只改 `game_engine.py`，同步更新测试断言
- 所有 AI 生成代码必须通过现有集成测试，不得破坏已有功能
