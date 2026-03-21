# 编译与运行说明

## 编译命令

### Windows (MinGW/MSYS2)
```bash
cd C:\Users\34764\Sprint
g++ -std=c++11 -o dungeon.exe main.cpp map.cpp player.cpp enemy.cpp game.cpp
```

### Windows (MSVC - 开发者命令行)
```batch
cd C:\Users\34764\Sprint
cl /EHsc /std:c++11 main.cpp map.cpp player.cpp enemy.cpp game.cpp
```

### Linux / macOS
```bash
g++ -std=c++11 -o dungeon main.cpp map.cpp player.cpp enemy.cpp game.cpp
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
| W | 向上移动 |
| A | 向左移动 |
| S | 向下移动 |
| D | 向右移动 |
| Q | 退出游戏 |

## 游戏规则

1. **目标**: 消灭所有敌人 (初始 3 个)
2. **玩家**: HP 20, 攻击力 5
3. **敌人**: HP 10, 攻击力 3
4. **战斗**: 接触敌人自动触发回合制战斗
5. **胜利**: 全灭敌人
6. **失败**: HP 归零

## 图例

| 符号 | 含义 |
|------|------|
| @ | 玩家 |
| E | 敌人 |
| # | 墙 |
| . | 地板 |

## 截图指南

### 截取游戏运行截图
1. 编译游戏：`g++ -std=c++11 -o dungeon.exe *.cpp`
2. 运行游戏：`dungeon.exe`
3. 按 Enter 开始游戏
4. 使用 WASD 移动
5. 使用 `Win + Shift + S` (Windows) 或 `Cmd + Shift + 4` (Mac) 截图

### 需要提交的截图
1. 游戏主界面截图（显示地图、玩家、敌人）
2. 战斗日志截图（显示战斗信息）
3. 游戏结束截图（胜利或失败画面）

## 常见问题

### Q: 编译时提示找不到文件
A: 确保所有 `.cpp` 文件在同一目录下

### Q: 中文显示乱码
A: 当前版本使用英文避免编码问题

### Q: 闪屏严重
A: `system("cls")` 可能导致闪烁，属正常现象

### Q: 输入无响应
A: 确保在游戏运行时按下 Enter 确认输入
