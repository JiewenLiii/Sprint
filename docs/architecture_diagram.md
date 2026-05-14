# 🎮 Sprint 项目架构图 - Mermaid 版本

## 1. 完整架构图 (带标注)

```mermaid
graph TB
    %% 样式定义
    classDef dataSource fill:#d3f9d8,stroke:#2b8a3e,stroke-width:3px,color:#2b8a3e
    classDef dataHub fill:#ffe3e3,stroke:#e03131,stroke-width:4px,color:#e03131
    classDef coreModule fill:#e7f5ff,stroke:#1e1e1e,stroke-width:2px,color:#1e1e1e
    classDef utilModule fill:#f3f3f3,stroke:#666,stroke-width:2px,color:#333
    classDef painPoint fill:#fff3bf,stroke:#d63384,stroke-width:2px,stroke-dasharray: 5 5,color:#d63384

    %% 数据源头
    Main["💾 main.cpp<br/>程序入口<br/>随机数种子"]:::dataSource
    Keyboard["💾 keyboard.h<br/>用户输入 WASD/Q<br/>⚠️ 难 Mock"]:::dataSource
    EnemyConfig["💾 enemy.cpp<br/>难度配置数据<br/>HP/ATK/数量"]:::dataSource

    %% 数据汇聚点 - Game 类
    subgraph GameModule [🟢 数据汇聚点 - Game 类]
        direction TB
        GameRun["Game::run()<br/>游戏主循环"]
        GameInit["initGame()<br/>初始化游戏"]
        GameHandle["handleInput()<br/>处理输入"]
        GameRender["render()<br/>渲染画面"]
        GameUpdate["updateEnemies()<br/>更新敌人"]
        GameCombat["combat()<br/>⚠️ 战斗逻辑硬编码"]
    end
    class GameModule dataHub

    %% 核心模块
    subgraph CoreModules [核心模块]
        direction TB
        Map["map.h/cpp<br/>• 地图渲染<br/>• 视野计算<br/>• 已探索记录"]:::coreModule
        Player["player.h/cpp<br/>• 玩家移动<br/>• 状态管理<br/>⚠️ #include enemy.h"]:::coreModule
        Enemy["enemy.h/cpp<br/>• 敌人 AI<br/>• 生成逻辑<br/>• 难度配置"]:::coreModule
    end

    %% 工具模块
    Colors["colors.h/cpp<br/>跨平台颜色<br/>Windows/Unix"]:::utilModule

    %% 函数调用关系
    Main --> GameRun
    Keyboard -.->|WASD 输入 | GameHandle
    GameRun --> GameInit
    GameRun --> GameHandle
    GameRun --> GameRender
    GameRun --> GameUpdate
    
    GameInit --> EnemyConfig
    GameInit --> Player
    GameInit --> Enemy
    
    GameHandle --> Player
    GameHandle --> GameCombat
    
    GameRender --> Map
    GameRender --> Player
    GameRender --> Enemy
    
    GameUpdate --> Enemy
    GameUpdate --> GameCombat
    
    GameCombat --> Player
    GameCombat --> Enemy

    %% 跨层调用 (虚线橙色)
    GameHandle -.->|⚠️ 跨层调用<br/>直接访问 x,y,hp | Player
    GameUpdate -.->|⚠️ 跨层调用<br/>直接访问 x,y,y | Enemy
    GameCombat -.->|⚠️ 跨层调用<br/>直接修改 hp | Player

    %% 循环依赖 (红色双向箭头)
    Player <-->|🔴 循环依赖 | Enemy

    %% 图例
    subgraph Legend [📋 图例]
        direction TB
        L1["─────► 函数调用"]
        L2["┈┈┈► 跨层调用"]
        L3["🟢 数据汇聚点"]
        L4["🔴 循环依赖"]
        L5["💾 数据源头"]
        L6["⚠️ 痛点节点"]
    end
    class Legend utilModule

    %% 痛点说明
    subgraph PainPoints [⚠️ 痛点节点总结]
        direction TB
        PP1["1. Game 类<br/>战斗逻辑硬编码<br/>改一处要改多处"]:::painPoint
        PP2["2. keyboard.h<br/>直接调用_getch()<br/>测试难 Mock"]:::painPoint
        PP3["3. Enemy 检测<br/>逻辑重复散布<br/>game.cpp + player.cpp"]:::painPoint
    end
```

---

## 2. 简化版架构图 (适合快速查看)

```mermaid
graph LR
    %% 样式
    classDef source fill:#d3f9d8,stroke:#2b8a3e,stroke-width:2px
    classDef hub fill:#ffe3e3,stroke:#e03131,stroke-width:3px
    classDef module fill:#e7f5ff,stroke:#1e1e1e,stroke-width:1px

    %% 数据流
    Main["💾 main.cpp"]:::source --> Game
    Keyboard["💾 keyboard.h"]:::source --> Game
    Config["💾 enemy.cpp"]:::source --> Game

    Game["🟢 Game 类<br/>数据汇聚点⭐"]:::hub --> Map
    Game --> Player
    Game --> Enemy

    Map["map.h/cpp<br/>地图渲染"]:::module
    Player["player.h/cpp<br/>玩家移动"]:::module
    Enemy["enemy.h/cpp<br/>敌人 AI"]:::module

    Player <-->|🔴 循环依赖 | Enemy

    subgraph 痛点
        Game -.->|改多处 | PP1[战斗逻辑]
        Keyboard -.->|难 Mock | PP2[测试困难]
        Player -.->|重复 | PP3[检测逻辑]
    end
```

---

## 3. 数据流向图

```mermaid
flowchart TD
    subgraph Input [📥 输入层 - 数据源头]
        I1["💾 main.cpp<br/>程序入口"]
        I2["💾 keyboard.h<br/>用户输入"]
        I3["💾 enemy.cpp<br/>难度配置"]
    end

    subgraph Process [⚙️ 处理层 - 数据汇聚]
        P1["🟢 Game::run()<br/>主循环"]
        P2["Game::handleInput()"]
        P3["Game::updateEnemies()"]
        P4["Game::combat()"]
    end

    subgraph Output [📤 输出层 - 渲染显示]
        O1["map.h/cpp<br/>地图渲染"]
        O2["colors.h/cpp<br/>颜色显示"]
    end

    subgraph State [💾 状态层]
        S1["player.h/cpp<br/>玩家状态"]
        S2["enemy.h/cpp<br/>敌人状态"]
    end

    I1 --> P1
    I2 --> P2
    I3 --> P3

    P1 --> P2
    P1 --> P3
    P2 --> P4

    P2 --> S1
    P3 --> S2
    P4 --> S1
    P4 --> S2

    S1 --> O1
    S2 --> O1
    O1 --> O2

    style Input fill:#d3f9d8
    style Process fill:#ffe3e3
    style Output fill:#e7f5ff
    style State fill:#fff3bf
```

---

## 4. 循环依赖详解图

```mermaid
graph LR
    subgraph PlayerModule [Player 模块]
        PH["player.h"]
        PC["player.cpp"]
        PM["Player::move()<br/>遍历 enemies<br/>检查碰撞"]
    end

    subgraph EnemyModule [Enemy 模块]
        EH["enemy.h"]
        EC["enemy.cpp"]
        EM["Enemy::isEnemyAt()<br/>遍历所有敌人<br/>O(n)检测"]
    end

    PH -->|🔴 #include | EH
    PC -->|依赖 | EM
    EC -->|需要玩家坐标 | PM

    style PlayerModule fill:#ffe3e3,stroke:#e03131
    style EnemyModule fill:#ffe3e3,stroke:#e0313e
```

---

## 5. 痛点节点鱼骨图

```mermaid
mindmap
  root((⚠️ 痛点节点))
    改一处改多处
      Game::combat()
        玩家先手逻辑
        敌人先手逻辑
        代码重复
      添加新武器
        改 combat()
        改 damage 计算
        改 UI 显示
    测试难 Mock
      keyboard.h
        直接调用_getch()
        无法注入测试
        需要真实输入
      Colors 类
        平台相关代码
        难以模拟输出
    逻辑重复散布
      敌人检测
        game.cpp
        player.cpp
        enemy.cpp
      碰撞检测
        Player::move()
        Enemy::moveTowardsPlayer()
```

---

## 📊 模块统计表格

| 模块名 | 类型 | 被依赖数 | 痛点等级 | 说明 |
|--------|------|---------|---------|------|
| main.cpp | 💾 数据源头 | 0 | - | 程序入口 |
| keyboard.h | 💾 数据源头 | 1 | 🔴🔴🔴 | 难 Mock |
| game.h/cpp | 🟢 数据汇聚点 | 5 | 🔴🔴🔴 | 逻辑重复 |
| map.h/cpp | 核心模块 | 3 | - | 渲染系统 |
| player.h/cpp | 核心模块 | 2 | 🔴🔴 | 循环依赖 |
| enemy.h/cpp | 核心模块 | 3 | 🔴🔴 | 循环依赖 + O(n) |
| colors.h/cpp | 工具模块 | 2 | 🔴 | 平台相关 |

---

## 🎯 重构优先级

```mermaid
pie title 重构优先级 (分数越高越优先)
    "keyboard.h - 添加接口层" : 30
    "Game::combat() - 提取战斗系统" : 25
    "Player/Enemy - 解耦循环依赖" : 20
    "Enemy 检测 - 统一 EntityRegistry" : 15
    "colors.h - 抽象显示接口" : 10
```

---

## 💡 改进建议

1. **创建 `IInputProvider` 接口**
   ```cpp
   class IInputProvider {
   public:
       virtual char getInput() = 0;
   };
   ```

2. **提取 `CombatSystem` 类**
   ```cpp
   class CombatSystem {
   public:
       CombatResult resolve(Player& p, Enemy& e);
   };
   ```

3. **创建 `EntityRegistry` 管理实体**
   ```cpp
   class EntityRegistry {
   public:
       bool isOccupied(int x, int y) const;
   };
   ```

---

**使用方法：**
- 在 VS Code 中安装 `Markdown Preview Mermaid Support` 插件
- 或在 https://mermaid.live/ 在线查看
- 或在 GitHub/GitLab 中直接渲染
