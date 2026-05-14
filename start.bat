@echo off
chcp 437 >nul
title Dungeon Adventure

echo ==============================================
echo   Dungeon Adventure - Starting...
echo ==============================================
echo.

set ROOT=%~dp0

echo [1/3] Starting backend (port 8080)...
start "Dungeon-Backend" /D "%ROOT%backend" cmd /k "python main.py"

timeout /t 3 /nobreak >nul

echo [2/3] Starting frontend (port 5173)...
start "Dungeon-Frontend" /D "%ROOT%frontend" cmd /k "npm run dev"

timeout /t 4 /nobreak >nul

echo [3/3] Opening browser...
start "" "http://localhost:5173"

echo.
echo ==============================================
echo   Done!
echo   Frontend: http://localhost:5173
echo   Backend:  http://localhost:8080
echo ==============================================
echo.
echo Press any key to close this window...
pause >nul
