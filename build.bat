@echo off
chcp 65001 >nul
echo ==============================================
echo 🔨 开始编译地牢探险游戏...
echo ==============================================
mkdir build 2>nul

g++ -std=c++11 -Wall ^
-I src ^
-I src/core ^
-I src/entities ^
-I src/world ^
-I src/ui ^
-I src/input ^  # 新增：补全input目录的头文件搜索路径
-o build/dungeon.exe ^
src/main.cpp ^
src/core/game.cpp src/core/battle.cpp ^
src/entities/entity.cpp src/entities/player.cpp src/entities/enemy.cpp ^
src/world/map.cpp ^
src/ui/renderer.cpp src/ui/colors.cpp

if %ERRORLEVEL% EQU 0 (
    echo ✅ 编译成功！
    echo 📂 游戏文件：build\dungeon.exe
) else (
    echo ❌ 编译失败！
)

echo.
echo 按任意键退出...
pause >nul