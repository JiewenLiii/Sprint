/**
 * 游戏主页面逻辑
 * 实现：游戏启动、地图渲染、玩家移动、状态查看
 */

// 页面加载完成后初始化
document.addEventListener('DOMContentLoaded', () => {
    console.log('DOM加载完成，开始初始化...');
    try {
        initButtons();
        console.log('按钮初始化完成');
        // 检查是否是从战斗页面返回，如果是则恢复游戏状态
        restoreGameStateIfNeeded();
        console.log('游戏状态恢复检查完成');
        
        // 如果不是从战斗返回且游戏未开始，则自动开始游戏
        if (!gameStarted) {
            console.log('游戏未开始，自动开始游戏');
            handleStartGame();
        } else {
            console.log('游戏已开始，跳过自动开始');
        }
        
        logOutput('log-output', '页面加载完成，等待操作...');
    } catch (error) {
        console.error('初始化错误:', error);
        logOutput('log-output', `初始化错误: ${error.message}`, 'error');
    }
});

/**
 * 初始化按钮事件
 */
function initButtons() {
    // 开始游戏
    document.getElementById('btn-start').addEventListener('click', handleStartGame);
    
    // 重新开始
    document.getElementById('btn-restart').addEventListener('click', handleRestartGame);
    
    // 移动控制
    document.getElementById('btn-up').addEventListener('click', () => handleMove('up'));
    document.getElementById('btn-down').addEventListener('click', () => handleMove('down'));
    document.getElementById('btn-left').addEventListener('click', () => handleMove('left'));
    document.getElementById('btn-right').addEventListener('click', () => handleMove('right'));

    // 查看状态
    const btnStatus = document.getElementById('btn-status');
    if (btnStatus) {
        btnStatus.addEventListener('click', handleGetPlayerStatus);
    }
}

/**
 * 检查是否需要恢复游戏状态
 */
function restoreGameStateIfNeeded() {
    // 检查URL参数是否有恢复标志
    const urlParams = new URLSearchParams(window.location.search);
    const restored = urlParams.get('restored');
    const playerHpFromUrl = urlParams.get('playerHp');
    
    if (restored === '1' && playerHpFromUrl) {
        // 尝试从localStorage获取之前保存的游戏状态
        const gameStateStr = localStorage.getItem('gameStateBeforeCombat');
        
        if (gameStateStr) {
            try {
                const gameState = JSON.parse(gameStateStr);
                
                // 恢复游戏状态
                currentPlayerPos = gameState.playerPos;
                // 过滤掉在战斗中被击败的敌人
                const defeatedEnemyIdRaw = localStorage.getItem('currentEncounteredEnemyId');
                // 解析JSON字符串，因为保存时使用了JSON.stringify
                const defeatedEnemyId = defeatedEnemyIdRaw ? JSON.parse(defeatedEnemyIdRaw) : null;
                enemies = gameState.enemies.filter(enemy => enemy.id !== defeatedEnemyId);
                gameStarted = gameState.gameStarted;
                
                // 清除已使用的数据
                localStorage.removeItem('currentEncounteredEnemyId');
                
                // 更新UI元素
                document.getElementById('player-id').textContent = gameState.playerData.id;
                document.getElementById('player-hp').textContent = gameState.playerData.hp;
                document.getElementById('player-attack').textContent = gameState.playerData.attack;
                document.getElementById('player-alive').textContent = gameState.playerData.alive;
                document.getElementById('player-pos').textContent = `(${gameState.playerPos[0]}, ${gameState.playerPos[1]})`;
                document.getElementById('enemy-count').textContent = enemies.length;
                
                // 显示面板
                document.getElementById('status-panel').style.display = 'block';
                
                // 构建地图渲染数据
                const mapRenderData = {
                    playerPosition: [...gameState.playerPos],
                    enemies: [...enemies],  // 使用过滤后的敌人列表
                    // 使用保存的地图数据
                    mapData: gameState.mapData ? gameState.mapData : null
                };
                
                // 渲染地图
                renderMap(mapRenderData);
                
                // 清除已使用的状态
                localStorage.removeItem('gameStateBeforeCombat');
                
                logOutput('log-output', `游戏状态已从战斗前恢复 - 位置(${gameState.playerPos[0]}, ${gameState.playerPos[1]})，血量${gameState.playerData.hp}，剩余敌人${enemies.length}个`, 'success');
            } catch (e) {
                logOutput('log-output', `恢复游戏状态失败: ${e.message}`, 'error');
                console.error(e);
            }
        } else {
            logOutput('log-output', '未找到保存的游戏状态数据', 'warning');
        }
    }
}

/**
 * 开始游戏
 */
async function handleStartGame() {
    console.log('开始游戏函数被调用');
    logOutput('log-output', '正在请求开始游戏...', 'info');
    
    try {
        const result = await GameAPI.startGame();
        console.log('API请求结果:', result);
        
        if (result.success) {
            logOutput('log-output', '游戏初始化成功！', 'success');
            displayGameInit(result.data);
        } else {
            logOutput('log-output', `开始游戏失败: ${result.error}`, 'error');
            logOutput('log-output', '使用默认游戏数据...', 'info');
            // 使用默认游戏数据
            displayGameInit({
                player: {
                    id: 'player-1',
                    hp: 100,
                    attack: 10,
                    isAlive: true
                },
                mapRender: {
                    playerPosition: [10, 10],
                    enemies: [
                        { id: 'enemy-1', position: [5, 5], hp: 20, attack: 5 },
                        { id: 'enemy-2', position: [15, 15], hp: 30, attack: 8 },
                        { id: 'enemy-3', position: [5, 15], hp: 25, attack: 6 },
                        { id: 'enemy-4', position: [15, 5], hp: 35, attack: 7 }
                    ]
                }
            });
        }
    } catch (error) {
        console.error('开始游戏错误:', error);
        logOutput('log-output', `开始游戏错误: ${error.message}`, 'error');
        logOutput('log-output', '使用默认游戏数据...', 'info');
        // 使用默认游戏数据
        displayGameInit({
            player: {
                id: 'player-1',
                hp: 100,
                attack: 10,
                isAlive: true
            },
            mapRender: {
                playerPosition: [10, 10],
                enemies: [
                    { id: 'enemy-1', position: [5, 5], hp: 20, attack: 5 },
                    { id: 'enemy-2', position: [15, 15], hp: 30, attack: 8 },
                    { id: 'enemy-3', position: [5, 15], hp: 25, attack: 6 },
                    { id: 'enemy-4', position: [15, 5], hp: 35, attack: 7 }
                ]
            }
        });
    }
}

/**
 * 重新开始游戏
 */
async function handleRestartGame() {
    logOutput('log-output', '正在请求重新开始游戏...', 'info');
    
    const result = await GameAPI.restartGame();
    
    if (result.success) {
        logOutput('log-output', '游戏重新开始成功！', 'success');
        displayGameInit(result.data);
    } else {
        logOutput('log-output', `重新开始失败: ${result.error}`, 'error');
    }
}

/**
 * 获取玩家状态
 */
async function handleGetPlayerStatus() {
    logOutput('log-output', '正在获取玩家状态...', 'info');
    
    const result = await GameAPI.getPlayerStatus();
    
    if (result.success) {
        logOutput('log-output', '玩家状态获取成功', 'success');
        displayPlayerStatus(result.data);
    } else {
        logOutput('log-output', `获取玩家状态失败: ${result.error}`, 'error');
    }
}

/**
 * 玩家位置状态管理（前端模拟）
 * Prism Mock 不会真正改变位置，前端自己维护
 */
let currentPlayerPos = [10, 10]; // 初始位置在中心
let enemies = []; // 敌人列表（从初始化获取）
let gameStarted = false; // 游戏是否已开始

/**
 * 移动玩家
 * @param {string} direction - up/down/left/right
 */
async function handleMove(direction) {
    // 边界测试：验证输入
    const errors = validateInput(direction, {
        required: true,
        type: 'string',
        enum: ['up', 'down', 'left', 'right']
    });

    if (errors.length > 0) {
        logOutput('log-output', `输入验证失败: ${errors.join(', ')}`, 'error');
        return;
    }

    if (!gameStarted) {
        logOutput('log-output', '请先点击「开始游戏」', 'error');
        return;
    }

    logOutput('log-output', `正在请求移动玩家: ${direction}...`, 'info');
    
    const result = await GameAPI.movePlayer(direction);
    
    if (result.success) {
        // 前端自己更新位置（因为 Mock 不会真正改变）
        updatePlayerPosition(direction);
        
        logOutput('log-output', `玩家移动成功: ${direction} → (${currentPlayerPos[0]}, ${currentPlayerPos[1]})`, 'success');
        displayMoveResult(result.data);
        
        // 检测是否遇到敌人
        if (checkEnemyEncounter()) {
            logOutput('log-output', '⚠️ 遇到敌人！自动进入战斗...', 'error');
            
            try {
                // 保存当前游戏状态到localStorage
                const gameState = {
                    playerPos: [...currentPlayerPos],
                    enemies: [...enemies],  // 保存当前敌人列表
                    gameStarted: gameStarted,
                    playerData: {
                        id: document.getElementById('player-id')?.textContent || '-',
                        hp: parseInt(document.getElementById('player-hp')?.textContent) || 100,
                        attack: parseInt(document.getElementById('player-attack')?.textContent) || 10,
                        alive: document.getElementById('player-alive')?.textContent || '✅ 存活'
                    },
                    mapData: getMapDataFromDisplay(), // 获取当前地图显示数据
                    enemyCount: parseInt(document.getElementById('enemy-count')?.textContent) || 0
                };
                
                localStorage.setItem('gameStateBeforeCombat', JSON.stringify(gameState));
                
                // 保存当前遭遇的敌人ID，用于后续移除
                const encounteredEnemy = getCurrentEncounteredEnemy();
                if (encounteredEnemy) {
                    localStorage.setItem('currentEncounteredEnemyId', JSON.stringify(encounteredEnemy.id));
                }
            } catch (error) {
                logOutput('log-output', `保存游戏状态时出错: ${error.message}`, 'error');
            }
            
            // 显示遇敌提示
            showEncounterAlert();
            
            // 延迟跳转到战斗页面（根据用户需求）
            logOutput('log-output', '2秒后跳转到战斗页面...', 'info');
            setTimeout(() => {
                window.location.href = 'combat.html';
            }, 2000); // 2秒延迟
        }
    } else {
        logOutput('log-output', `移动失败: ${result.error}`, 'error');
    }
}

/**
 * 获取当前地图显示数据
 */
function getMapDataFromDisplay() {
    // 返回当前地图的网格数据，包括玩家位置和敌人位置
    return {
        playerPosition: [...currentPlayerPos],
        enemies: [...enemies],
        // 注意：由于地图网格是动态渲染的，我们只保存关键状态数据
        // 在恢复时会重新渲染地图
    };
}

/**
 * 获取当前遇到的敌人
 */
function getCurrentEncounteredEnemy() {
    for (let i = 0; i < enemies.length; i++) {
        const enemy = enemies[i];
        if (!enemy.position || enemy.position.length < 2) continue;
        
        const dx = Math.abs(currentPlayerPos[0] - enemy.position[0]);
        const dy = Math.abs(currentPlayerPos[1] - enemy.position[1]);
        
        // 邻接（包括对角线）
        if (dx <= 1 && dy <= 1) {
            return enemy;
        }
    }
    return null;
}

/**
 * 检查是否遇到敌人（前端模拟）
 * 邻接判断：玩家和敌人坐标差 <= 1
 */
function checkEnemyEncounter() {
    for (let i = 0; i < enemies.length; i++) {
        const enemy = enemies[i];
        if (!enemy.position || enemy.position.length < 2) continue;
        
        const dx = Math.abs(currentPlayerPos[0] - enemy.position[0]);
        const dy = Math.abs(currentPlayerPos[1] - enemy.position[1]);
        
        // 邻接（包括对角线）
        if (dx <= 1 && dy <= 1) {
            logOutput('log-output', `检测到敌人 ${enemy.id || 'Unknown'} 在位置 (${enemy.position[0]}, ${enemy.position[1]})`, 'error');
            // 保存当前遭遇的敌人信息到 localStorage 供战斗页使用
            localStorage.setItem('currentEnemy', JSON.stringify(enemy));
            return true;
        }
    }
    return false;
}

/**
 * 显示遇敌提示
 */
function showEncounterAlert() {
    const alert = document.createElement('div');
    alert.className = 'alert alert-danger text-center';
    alert.innerHTML = '<strong>⚠️ 遇到敌人！即将进入战斗...</strong>';
    alert.style.position = 'fixed';
    alert.style.top = '20%';
    alert.style.left = '50%';
    alert.style.transform = 'translate(-50%, -50%)';
    alert.style.zIndex = '9999';
    alert.style.minWidth = '300px';
    alert.style.boxShadow = '0 0 20px rgba(218, 54, 51, 0.5)';
    
    document.body.appendChild(alert);
    
    // 2秒后移除
    setTimeout(() => {
        alert.remove();
    }, 2000);
}

/**
 * 更新玩家位置（前端维护）
 */
function updatePlayerPosition(direction) {
    const [x, y] = currentPlayerPos;
    
    switch (direction) {
        case 'up':
            currentPlayerPos = [x, Math.max(1, y - 1)];
            break;
        case 'down':
            currentPlayerPos = [x, Math.min(18, y + 1)];
            break;
        case 'left':
            currentPlayerPos = [Math.max(1, x - 1), y];
            break;
        case 'right':
            currentPlayerPos = [Math.min(18, x + 1), y];
            break;
    }
}

/**
 * 显示游戏初始化数据
 */
function displayGameInit(data) {
    console.log('显示游戏初始化数据:', data);
    // 重置状态
    currentPlayerPos = [10, 10];
    gameStarted = true;
    
    // 保存敌人列表（用于遇敌检测）
    enemies = [];
    if (data.mapRender && data.mapRender.enemies) {
        // 使用原始敌人数据，而不是重新随机生成
        const originalEnemies = data.mapRender.enemies;
        enemies = originalEnemies.map(e => {
            // 复制敌人对象
            const enemyCopy = {...e};
            // 使用原始位置或随机位置（仅在位置缺失时）
            if (!enemyCopy.position || enemyCopy.position.length < 2) {
                // 随机生成敌人位置（2-17之间，避开边缘）
                enemyCopy.position = [
                    Math.floor(Math.random() * 16) + 2, 
                    Math.floor(Math.random() * 16) + 2
                ];
                // 确保敌人不在玩家初始位置
                if (enemyCopy.position[0] === 10 && enemyCopy.position[1] === 10) {
                    enemyCopy.position = [9, 9]; // 设置一个替代位置
                }
            }
            return enemyCopy;
        });
        logOutput('log-output', `检测到 ${enemies.length} 个敌人，分布在地图上`, 'info');
    }
    
    // 显示状态面板
    document.getElementById('status-panel').style.display = 'block';

    // 显示玩家信息
    if (data.player) {
        console.log('更新玩家信息:', data.player);
        document.getElementById('player-id').textContent = data.player.id || '-';
        document.getElementById('player-hp').textContent = data.player.hp || '-';
        document.getElementById('player-attack').textContent = data.player.attack || '-';
        // Player模型没有isAlive字段，默认显示存活
        document.getElementById('player-alive').textContent = '✅ 存活';
        document.getElementById('player-pos').textContent = `(10, 10)`;
    }

    // 显示地图信息
    if (data.mapRender) {
        console.log('渲染地图:', data.mapRender);
        // 创建副本以避免修改原始数据
        const mapRenderData = {...data.mapRender};
        // 替换为前端维护的位置
        mapRenderData.playerPosition = [...currentPlayerPos];
        mapRenderData.enemies = [...enemies]; // 使用我们生成的敌人列表
        renderMap(mapRenderData);
        document.getElementById('enemy-count').textContent = enemies.length;
    }
}

/**
 * 显示玩家状态
 */
function displayPlayerStatus(data) {
    document.getElementById('status-panel').style.display = 'block';
    
    document.getElementById('player-id').textContent = data.id || '-';
    document.getElementById('player-hp').textContent = data.hp || '-';
    document.getElementById('player-attack').textContent = data.attack || '-';
    // 只有PlayerStatusResponse有isAlive字段
    document.getElementById('player-alive').textContent = 
        data.isAlive !== undefined ? (data.isAlive ? '✅ 存活' : '❌ 已死亡') : '✅ 存活';
}

/**
 * 显示移动结果
 */
function displayMoveResult(data) {
    // 使用前端维护的位置
    document.getElementById('player-pos').textContent = 
        `(${currentPlayerPos[0]}, ${currentPlayerPos[1]})`;
    
    if (data.playerStatus) {
        displayPlayerStatus(data.playerStatus);
    }
    
    // 重新渲染地图（带新位置）
    if (data.mapRender) {
        // 创建副本，避免修改原始数据
        const mapRenderData = {...data.mapRender};
        // 替换 playerPosition 为前端维护的真实位置
        mapRenderData.playerPosition = [...currentPlayerPos];
        // 使用当前的敌人列表，而不是可能被修改的原始数据
        mapRenderData.enemies = [...enemies];
        renderMap(mapRenderData);
    }
}

/**
 * 数据增强层 - 处理 Prism Mock 返回的不完整数据
 * Prism 只返回最小数组（长度=1），这里补全为实际游戏数据
 */
function enhanceMapData(rawData) {
    if (!rawData) return null;

    // 创建副本，避免修改原始数据
    const enhanced = JSON.parse(JSON.stringify(rawData));

    // 玩家位置：如果只有 1 个值，补全为 [x, y]
    if (Array.isArray(enhanced.playerPosition) && enhanced.playerPosition.length === 1) {
        enhanced.playerPosition = [enhanced.playerPosition[0] || 10, 10];
    }

    // 敌人位置：同样处理
    if (Array.isArray(enhanced.enemies)) {
        enhanced.enemies = enhanced.enemies.map(e => {
            if (Array.isArray(e.position) && e.position.length === 1) {
                e.position = [e.position[0] + 5, 5]; // 敌人放在不同位置
            }
            return e;
        });
    }

    // 地图网格：如果只有 1x1，补全为 20x20
    if (!Array.isArray(enhanced.mapData) || enhanced.mapData.length < 2) {
        enhanced.mapData = generateDefaultMap(enhanced.playerPosition);
    }

    return enhanced;
}

/**
 * 生成默认 20x20 地图（模拟数据）
 */
function generateDefaultMap(playerPos) {
    const map = [];
    for (let y = 0; y < 20; y++) {
        const row = [];
        for (let x = 0; x < 20; x++) {
            // 边缘是墙，中间是地板
            if (x === 0 || x === 19 || y === 0 || y === 19) {
                row.push(1); // 墙
            } else {
                row.push(0); // 地板
            }
        }
        map.push(row);
    }
    return map;
}

/**
 * 渲染地图
 * 边界测试：空数据、异常数据结构
 */
function renderMap(mapData) {
    const container = document.getElementById('map-container');
    container.innerHTML = '';

    if (!mapData) {
        container.innerHTML = '<p class="text-center text-muted">暂无地图数据</p>';
        return;
    }

    // 数据增强
    const data = enhanceMapData(mapData);

    const grid = data.mapData || [];
    const playerPos = data.playerPosition || [10, 10];
    const enemies = data.enemies || [];

    // 更新显示
    document.getElementById('player-pos').textContent = `(${playerPos[0]}, ${playerPos[1]})`;
    document.getElementById('enemy-count').textContent = enemies.length;

    // 渲染网格
    if (!Array.isArray(grid) || grid.length < 2) {
        // 生成默认地图
        const defaultMap = generateDefaultMap(playerPos);
        for (let y = 0; y < 20; y++) {
            for (let x = 0; x < 20; x++) {
                createMapCell(x, y, playerPos, enemies, container, defaultMap[y][x]);
            }
        }
    } else {
        for (let y = 0; y < grid.length && y < 20; y++) {
            const row = grid[y] || [];
            for (let x = 0; x < 20; x++) {
                createMapCell(x, y, playerPos, enemies, container, row[x] || 0);
            }
        }
    }
}

/**
 * 创建单个地图格子
 */
function createMapCell(x, y, playerPos, enemies, container, terrainType) {
    const cell = document.createElement('div');
    cell.className = 'map-cell';

    // 判断是否是玩家位置
    if (playerPos[0] === x && playerPos[1] === y) {
        cell.classList.add('player');
        cell.textContent = '@';
    }
    // 判断是否是敌人位置
    else if (enemies.some(e => {
        const pos = e.position;
        return Array.isArray(pos) && pos[0] === x && pos[1] === y;
    })) {
        cell.classList.add('enemy');
        cell.textContent = 'E';
    }
    // 普通地形
    else {
        cell.classList.add('floor');
        cell.textContent = terrainType === 1 ? '#' : '.';
    }

    container.appendChild(cell);
}

/**
 * 显示错误信息
 */
function showError(elementId, message) {
    let errorEl = document.getElementById(elementId);
    if (!errorEl) {
        errorEl = document.createElement('div');
        errorEl.id = elementId;
        errorEl.className = 'alert alert-danger mt-2';
        document.querySelector('.container').appendChild(errorEl);
    }
    errorEl.textContent = message;
    errorEl.style.display = 'block';
    
    // 3秒后自动隐藏
    setTimeout(() => {
        errorEl.style.display = 'none';
    }, 3000);
}