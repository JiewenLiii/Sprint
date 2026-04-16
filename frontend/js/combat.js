/**
 * 战斗页面逻辑
 * 实现：战斗发起、战斗结果展示、血量条更新、边界测试
 */

// 页面加载完成后初始化
document.addEventListener('DOMContentLoaded', () => {
    initCombatButtons();
    logOutput('log-output', '战斗页面加载完成，等待操作...');
    
    // 页面加载后立即开始战斗
    handleStartCombat();
});

/**
 * 初始化战斗按钮
 */
function initCombatButtons() {
    // 隐藏开始战斗按钮，因为我们现在自动开始战斗
    const startButton = document.getElementById('btn-combat-start');
    if(startButton) {
        startButton.style.display = 'none';
    }
    
    // 获取确认返回按钮并绑定事件
    const confirmReturnBtn = document.getElementById('confirm-return-btn');
    if(confirmReturnBtn) {
        confirmReturnBtn.addEventListener('click', confirmReturnToMap);
    }
    
    // 更新战斗控制区域提示
    const combatControlDiv = document.getElementById('combat-loading');
    if(combatControlDiv) {
        combatControlDiv.textContent = '⚔️ 战斗正在进行中...';
        combatControlDiv.className = 'text-warning'; // 添加警告样式表示正在进行
    }
}

/**
 * 开始战斗
 * 边界测试：
 * 1. 模拟玩家未初始化就发起战斗
 * 2. 模拟网络异常（超时/断开）
 * 3. 模拟返回异常数据结构
 */
async function handleStartCombat() {
    const btn = document.getElementById('btn-combat-start');
    
    // 边界测试：防止重复点击
    if (btn && btn.disabled) {
        logOutput('log-output', '战斗正在进行中，请勿重复点击', 'error');
        return;
    }

    // 禁用按钮
    if(btn) {
        btn.disabled = true;
        btn.textContent = '战斗进行中...';
    }

    logOutput('log-output', '正在请求开始战斗...', 'info');

    try {
        const result = await GameAPI.startCombat();

        if (result.success) {
            logOutput('log-output', '战斗开始，模拟战斗过程...', 'success');
            // 前端模拟战斗扣血过程
            await simulateCombat(result.data);
        } else {
            logOutput('log-output', `战斗请求失败: ${result.error}`, 'error');
            
            // 边界测试：网络异常处理
            if (result.error.includes('NetworkError') || result.error.includes('Failed to fetch')) {
                logOutput('log-output', '⚠️ 边界测试：检测到网络异常，请确保 Prism Mock 服务已启动', 'error');
                logOutput('log-output', '提示：运行 prism mock dungeon-adventure.yaml 启动服务', 'error');
                
                // 如果出错，恢复初始提示
                const combatControlDiv = document.getElementById('combat-loading');
                if(combatControlDiv) {
                    combatControlDiv.textContent = '战斗即将开始...';
                    combatControlDiv.className = 'text-muted';
                }
            }
        }
    } catch (error) {
        logOutput('log-output', `战斗异常: ${error.message}`, 'error');
        // 只有报错时才恢复按钮
        if(btn) {
            btn.disabled = false;
            btn.textContent = '开始战斗';
        }
    }
}

/**
 * 模拟战斗过程（前端动画）
 * 因为 Prism Mock 只返回静态数据，需要前端自己模拟扣血
 */
async function simulateCombat(initialData) {
    let playerHp = initialData.player?.hp || 100;
    let enemyHp = initialData.enemy?.hp || 50;
    const playerAtk = initialData.player?.attack || 10;
    const enemyAtk = initialData.enemy?.attack || 5;
    let round = 1;
    
    // 初始化战斗统计
    let totalPlayerDamage = 0;
    let totalEnemyDamage = 0;
    let playerHits = 0;
    let enemyHits = 0;

    // 初始化显示（立刻同步到面板）
    updateCombatInfo(initialData, playerHp, enemyHp);
    document.getElementById('combat-player-id').textContent = initialData.player?.id || 'player_001';
    document.getElementById('combat-enemy-id').textContent = initialData.enemy?.id || 'enemy_001';
    document.getElementById('combat-player-attack').textContent = playerAtk;
    document.getElementById('combat-enemy-attack').textContent = enemyAtk;
    logOutput('log-output', `⚔️ 遭遇战开始！${initialData.player?.id || '玩家'} VS ${initialData.enemy?.id || '敌人'}`);
    
    // 隐藏上一次的结果弹窗
        document.getElementById('combat-result-modal').style.display = 'none';

    // 死斗模式：直到一方倒下
    while (playerHp > 0 && enemyHp > 0) {
        logOutput('log-output', `--- 第 ${round} 轮 ---`);
        
        // 玩家攻击
        const playerDmg = Math.max(1, Math.floor(playerAtk * (0.8 + Math.random() * 0.4)));
        enemyHp = Math.max(0, enemyHp - playerDmg);  // 确保不会出现负数血量
        totalPlayerDamage += playerDmg;
        playerHits++;
        logOutput('log-output', `⚔️ 玩家攻击，造成 ${playerDmg} 伤害 → 敌人剩余 HP: ${enemyHp}`);
        updateCombatInfo(initialData, playerHp, enemyHp);
        await sleep(800);
        
        // 检查敌人是否已死
        if (enemyHp <= 0) {
            logOutput('log-output', '💀 敌人已被消灭！战斗结束！', 'success');
            break;
        }
        
        // 敌人反击
        const enemyDmg = Math.max(1, Math.floor(enemyAtk * (0.8 + Math.random() * 0.4)));
        playerHp = Math.max(0, playerHp - enemyDmg);  // 确保不会出现负数血量
        totalEnemyDamage += enemyDmg;
        enemyHits++;
        logOutput('log-output', `👹 敌人反击，造成 ${enemyDmg} 伤害 → 玩家剩余 HP: ${playerHp}`);
        updateCombatInfo(initialData, playerHp, enemyHp);
        await sleep(800);
        
        // 检查玩家是否已死
        if (playerHp <= 0) {
            logOutput('log-output', '💀 你已被敌人消灭！战斗结束！', 'error');
            break;
        }

        // 保护机制：防止意外死循环，最多打 20 轮
        if (round > 20) {
            logOutput('log-output', '⚠️ 战斗过于激烈，判定为平局！');
            playerHp = 1; 
            enemyHp = 1;
            break;
        }

        round++;
    }

    // 判定结果（只有 win 或 lose）
    let result = enemyHp <= 0 ? 'win' : 'lose';

    // 显示最终结果
    displayCombatResult({
        player: { ...initialData.player, hp: playerHp },
        enemy: { ...initialData.enemy, hp: enemyHp },
        result: result
    }, {
        totalPlayerDamage,
        totalEnemyDamage,
        playerHits,
        enemyHits,
        rounds: round
    });

    // 更新战斗控制区域提示
    const combatControlDiv = document.getElementById('combat-loading');
    if(combatControlDiv) {
        combatControlDiv.textContent = '战斗结束，点击下方按钮返回地图';
        combatControlDiv.className = 'text-success'; // 使用成功样式表示已完成
    }
    
    // 显示确认返回按钮
    const confirmReturnBtn = document.getElementById('confirm-return-btn');
    if(confirmReturnBtn) {
        confirmReturnBtn.style.display = 'inline-block';
    }
    
    logOutput('log-output', '战斗结束，等待返回地图...', 'info');
}

/**
 * 更新战斗信息（实时更新）
 */
function updateCombatInfo(data, currentPlayerHp, currentEnemyHp) {
    if (data.player) {
        document.getElementById('combat-player-id').textContent = data.player.id || 'player_001';
        document.getElementById('combat-player-attack').textContent = data.player.attack || '10';

        const playerHpPercent = (currentPlayerHp / 100) * 100;
        document.getElementById('player-hp-bar').style.width = `${playerHpPercent}%`;
        document.getElementById('combat-player-hp').textContent = Math.max(0, currentPlayerHp); // 确保不显示负数
    }

    if (data.enemy) {
        document.getElementById('combat-enemy-id').textContent = data.enemy.id || 'enemy_001';
        document.getElementById('combat-enemy-attack').textContent = data.enemy.attack || '5';

        const enemyHpPercent = Math.max(0, (currentEnemyHp / 50) * 100);
        document.getElementById('enemy-hp-bar').style.width = `${enemyHpPercent}%`;
        document.getElementById('combat-enemy-hp').textContent = Math.max(0, currentEnemyHp); // 确保不显示负数
    }
}

/**
 * 延迟函数
 */
function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

/**
 * 显示战斗结果
 */
function displayCombatResult(data, stats = {}) {
    // 显示结果弹窗
    document.getElementById('combat-result-modal').style.display = 'block';

    // 更新玩家信息
    if (data.player) {
        document.getElementById('combat-player-id').textContent = data.player.id || '-';
        document.getElementById('combat-player-hp').textContent = (typeof data.player.hp !== 'undefined' && data.player.hp !== null) ? Math.max(0, data.player.hp) : '-'; // 确保不显示负数
        document.getElementById('combat-player-attack').textContent = data.player.attack || '-';
        
        // 更新血量条（假设满血为 100）
        const playerHpPercent = (typeof data.player.hp !== 'undefined' && data.player.hp !== null ? Math.max(0, data.player.hp) : 0) / 100 * 100;
        document.getElementById('player-hp-bar').style.width = `${playerHpPercent}%`;

        document.getElementById('result-player-hp').textContent = (typeof data.player.hp !== 'undefined' && data.player.hp !== null) ? Math.max(0, data.player.hp) : '-';
    }

    // 更新敌人信息
    if (data.enemy) {
        document.getElementById('combat-enemy-id').textContent = data.enemy.id || '-';
        document.getElementById('combat-enemy-hp').textContent = (typeof data.enemy.hp !== 'undefined' && data.enemy.hp !== null) ? Math.max(0, data.enemy.hp) : '-'; // 确保不显示负数
        document.getElementById('combat-enemy-attack').textContent = data.enemy.attack || '-';
        
        // 更新血量条（假设满血为 50）
        const enemyHpPercent = (typeof data.enemy.hp !== 'undefined' && data.enemy.hp !== null ? Math.max(0, data.enemy.hp) : 0) / 50 * 100;
        document.getElementById('enemy-hp-bar').style.width = `${enemyHpPercent}%`;
    }

    // 更新战斗统计信息
    if(stats) {
        document.getElementById('total-player-damage').textContent = stats.totalPlayerDamage || 0;
        document.getElementById('total-enemy-damage').textContent = stats.totalEnemyDamage || 0;
        document.getElementById('player-hits').textContent = stats.playerHits || 0;
        document.getElementById('enemy-hits').textContent = stats.enemyHits || 0;
        document.getElementById('total-rounds').textContent = stats.rounds || 0;
    }

    // 显示战斗结果
    if (data.result) {
        const resultText = document.getElementById('result-text');
        switch (data.result) {
            case 'win':
                resultText.textContent = '🎉 胜利！';
                resultText.className = 'text-success';
                logOutput('log-output', '战斗结果：胜利！', 'success');
                break;
            case 'lose':
                resultText.textContent = '💀 失败！';
                resultText.className = 'text-danger';
                logOutput('log-output', '战斗结果：失败！', 'error');
                break;
            case 'flee':
                resultText.textContent = '🏃 逃跑成功！';
                resultText.className = 'text-warning';
                logOutput('log-output', '战斗结果：逃跑成功！', 'info');
                break;
            default:
                resultText.textContent = `未知结果: ${data.result}`;
                resultText.className = 'text-muted';
                logOutput('log-output', `边界测试：未知的战斗结果类型: ${data.result}`, 'error');
        }
    }

    // 显示确认返回按钮
    const confirmReturnBtn = document.getElementById('confirm-return-btn');
    if(confirmReturnBtn) {
        confirmReturnBtn.style.display = 'inline-block';
    }

    logOutput('log-output', '战斗数据已展示', 'success');
}

/**
 * 点击确认返回地图，使用现有状态（战斗后状态）
 */
function confirmReturnToMap() {
    // 获取当前战斗后的玩家血量
    const finalPlayerHp = document.getElementById('result-player-hp').textContent;
    
    // 跳转到主页，并携带战斗后的玩家血量
    // 对参数进行编码以确保URL安全
    window.location.href = `index.html?restored=1&playerHp=${encodeURIComponent(finalPlayerHp)}`;
}

/**
 * 边界测试：手动测试异常场景
 */
function runBoundaryTests() {
    logOutput('log-output', '=== 开始边界测试 ===', 'info');

    // 测试 1：空数据结构
    logOutput('log-output', '测试 1：处理空数据结构...', 'info');
    try {
        displayCombatResult({});
        logOutput('log-output', '✅ 测试 1 通过：空数据已安全处理', 'success');
    } catch (error) {
        logOutput('log-output', `❌ 测试 1 失败: ${error.message}`, 'error');
    }

    // 测试 2：异常数据类型
    logOutput('log-output', '测试 2：处理异常数据类型...', 'info');
    try {
        displayCombatResult({
            player: { hp: 'invalid', attack: null },
            enemy: { hp: -100, attack: 'abc' },
            result: 'unknown_result'
        });
        logOutput('log-output', '✅ 测试 2 通过：异常数据已安全处理', 'success');
    } catch (error) {
        logOutput('log-output', `❌ 测试 2 失败: ${error.message}`, 'error');
    }

    // 测试 3：网络超时模拟
    logOutput('log-output', '测试 3：网络超时处理...', 'info');
    logOutput('log-output', '提示：关闭 Prism 服务后点击"开始战斗"按钮测试', 'info');

    logOutput('log-output', '=== 边界测试完成 ===', 'success');
}

// 页面加载 5 秒后自动运行边界测试（可选，演示用）
// setTimeout(runBoundaryTests, 5000);