# 编译与运行说明

## 编译命令

### Windows (MinGW/MSYS2)
```bash
cd C:\Users\34764\Sprint
g++ -std=c++11 -o dungeon.exe main.cpp map.cpp player.cpp enemy.cpp game.cpp colors.cpp
```

### Windows (MSVC - 开发者命令行)
```batch
cd C:\Users\34764\Sprint
cl /EHsc /std:c++11 main.cpp map.cpp player.cpp enemy.cpp game.cpp colors.cpp
```

### Linux / macOS
```bash
g++ -std=c++11 -o dungeon main.cpp map.cpp player.cpp enemy.cpp game.cpp colors.cpp
```

## 运行游戏

### Windows
```bash
dungeon.exe
```

### Linux / macOS
```bash
./dungeon
```

## 游戏操作

| 按键 | 功能 |
|------|------|
| **W** | 向上移动 |
| **A** | 向左移动 |
| **S** | 向下移动 |
| **D** | 向右移动 |
| **Q** | 返回主菜单 |

## 游戏规则

1. **目标**: 消灭所有敌人
2. **玩家**: HP 20, 攻击力 5
3. **地图**: 20×20，圆形视野（半径 6）
4. **战斗**: 相邻（上下左右）自动触发回合制战斗
5. **胜利**: 全灭敌人
6. **失败**: HP 归零

## 难度设置

| 难度 | 敌人数量 | 敌人 HP | 敌人攻击 |
|------|---------|--------|---------|
| **Easy** | 4 | 10 | 3 |
| **Hard** | 8 | 15 | 6 |

## 图例

| 符号 | 含义 |
|------|------|
| `@` | 玩家（亮绿色） |
| `E` | 敌人（红色系，4 种颜色循环） |
| `#` | 墙壁 |
| `.` | 地板 |
| `?` | 未探索区域 |
| `:` | 已探索地板（暗色） |
| `*` | 玩家足迹（绿色渐变，最近 2 步亮绿，其余普通绿） |
| `,` | 敌人足迹（黄色渐变，最近 2 步亮黄，其余普通黄，每个敌人独立显示 5 个） |

## 视野系统

- **可见区域**: 距离玩家 ≤ 6 格的圆形范围
- **已探索区域**: 曾经可见的区域，显示为暗色
- **未探索区域**: 从未见过的区域，显示为 `?`

## 截图指南

### 截取游戏运行截图
1. 编译游戏：`g++ -std=c++11 -o dungeon.exe *.cpp`
2. 运行游戏：`dungeon.exe`
3. 选择难度开始游戏
4. 使用 WASD 移动探索
5. 使用 `Win + Shift + S` (Windows) 或 `Cmd + Shift + 4` (Mac) 截图

### 需要提交的截图
1. 游戏主界面截图（显示地图、玩家、敌人）
2. 战斗界面截图（显示战斗信息）
3. 游戏结束截图（胜利或失败画面）
4. 主菜单截图（显示难度选择）

## 推荐设置

- **窗口大小**: 80×40 字符
- **字体**: Consolas 或 Lucida Console（等宽字体）

## 常见问题

### Q: 编译时提示找不到文件
A: 确保所有 `.cpp` 文件在同一目录下

### Q: 中文显示乱码
A: 确保控制台使用 UTF-8 编码（Windows 可使用 `chcp 65001`）

### Q: 颜色显示异常
A: Windows 10+ 支持最佳，或使用 ConEmu 等终端

### Q: 输入无响应
A: 确保在游戏运行时直接按键，无需按 Enter 确认

### Q: 视野太小/太大
A: 可修改 `map.h` 中的 `VIEW_RADIUS` 值（默认 6）

### Q: 地图显示不完整
A: 调整控制台窗口大小至 80×40 或更大
