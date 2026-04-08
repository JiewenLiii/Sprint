# User Stories - Sprint 1

## 核心用户故事池 (MVP)

### US-001: 玩家状态查看
**As a** player,  
**I want to** view my status (HP, Attack),  
**So that** I know my ability.

- **优先级**: P0
- **验收标准**:
  - 游戏界面始终显示当前 HP 和攻击力
  - HP 格式为 "当前/最大" (如 15/20)
- **状态**: ✅ 已完成

---

### US-002: 地牢探索移动
**As a** player,  
**I want to** move between rooms by entering commands (WASD),  
**So that** I can explore the dungeon.

- **优先级**: P0
- **验收标准**:
  - W/A/S/D 分别对应上/左/下/右移动
  - 不能移动到墙 (#) 上
  - 不能移出地图边界
- **状态**: ✅ 已完成

---

### US-003: 环境观察
**As a** player,  
**I want to** look around the current room,  
**So that** I can see the environment and monsters.

- **优先级**: P0
- **验收标准**:
  - 地图清晰显示墙 (#)、地板 (.)、玩家 (@)、敌人 (E)
  - 每次移动后自动刷新显示
- **状态**: ✅ 已完成

---

### US-004: 怪物战斗
**As a** player,  
**I want to** attack monsters in the dungeon,  
**So that** I can defeat enemies and get rewards.

- **优先级**: P0
- **验收标准**:
  - 接触敌人时自动进入战斗
  - 回合制战斗，双方轮流攻击
  - 显示战斗日志（伤害数值）
  - 敌人死亡后从地图消失
- **状态**: ✅ 已完成

---

### US-005: 游戏结束判定
**As a** player,  
**I want to** know when the game ends (win/lose),  
**So that** I can understand the outcome.

- **优先级**: P0
- **验收标准**:
  - 玩家 HP=0 时显示"游戏结束"
  - 全灭敌人时显示"胜利"
- **状态**: ✅ 已完成

---

## 可选用户故事 (P1/P2)

### US-101: 物品拾取
**As a** player,  
**I want to** pick up items (potions, weapons),  
**So that** I can restore HP or increase attack.

- **优先级**: P1
- **状态**: ⏳ 待实现

---

### US-102: 多种敌人类型
**As a** player,  
**I want to** encounter different types of enemies,  
**So that** the game is more challenging.

- **优先级**: P1
- **状态**: ⏳ 待实现

---

## 任务看板链接

GitHub Projects: [填写看板链接]

```
┌─────────────────────────────────────────────────────────────┐
│                        Kanban Board                         │
├──────────────┬──────────────┬──────────────┬───────────────┤
│   To Do      │  In Progress │    Review    │    Done       │
├──────────────┼──────────────┼──────────────┼───────────────┤
│ US-101 物品  │              │              │ US-001 状态   │
│ US-102 多敌人│              │              │ US-002 移动   │
│              │              │              │ US-003 观察   │
│              │              │              │ US-004 战斗   │
│              │              │              │ US-005 结束   │
└──────────────┴──────────────┴──────────────┴───────────────┘
```

---

**更新日期**: 2026-03-21
