/**
 * API 调用封装
 * 对接 Prism Mock 服务 (http://127.0.0.1:4010)
 */

const API_BASE = 'http://127.0.0.1:4010';

/**
 * 通用请求函数
 * @param {string} path - 接口路径
 * @param {string} method - 请求方法
 * @param {object} body - 请求体（POST 用）
 * @returns {Promise<object>} 响应数据
 */
async function apiRequest(path, method = 'GET', body = null) {
    const options = {
        method: method,
        headers: {
            'Content-Type': 'application/json'
        }
    };

    if (body && method === 'POST') {
        options.body = JSON.stringify(body);
    }

    try {
        const response = await fetch(`${API_BASE}${path}`, options);
        
        if (!response.ok) {
            throw new Error(`HTTP 错误: ${response.status} ${response.statusText}`);
        }

        const data = await response.json();
        return { success: true, data: data };
    } catch (error) {
        console.error(`API 请求失败 [${method} ${path}]:`, error);
        return { success: false, error: error.message };
    }
}

/**
 * 游戏相关 API
 */
const GameAPI = {
    // 开始游戏
    async startGame() {
        return await apiRequest('/game/start', 'POST');
    },

    // 重新开始游戏
    async restartGame() {
        return await apiRequest('/game/restart', 'POST');
    },

    // 玩家移动
    async movePlayer(direction) {
        const validDirections = ['up', 'down', 'left', 'right'];
        if (!validDirections.includes(direction)) {
            return { success: false, error: `无效的方向: ${direction}，必须是 ${validDirections.join(', ')}` };
        }
        return await apiRequest('/game/player/move', 'POST', { direction: direction });
    },

    // 开始战斗
    async startCombat() {
        return await apiRequest('/game/combat/start', 'POST');
    },

    // 获取地图渲染数据
    async getMapRender() {
        return await apiRequest('/game/map/render', 'GET');
    },

    // 获取玩家状态
    async getPlayerStatus() {
        return await apiRequest('/game/player/status', 'GET');
    }
};

/**
 * 日志输出工具
 */
function logOutput(elementId, message, type = 'info') {
    const logBox = document.getElementById(elementId);
    if (!logBox) return;

    const timestamp = new Date().toLocaleTimeString();
    const prefix = type === 'error' ? '❌' : type === 'success' ? '✅' : 'ℹ️';
    logBox.textContent += `[${timestamp}] ${prefix} ${message}\n`;
    logBox.scrollTop = logBox.scrollHeight;
}

/**
 * 边界测试工具
 */
function validateInput(value, rules) {
    const errors = [];

    if (rules.required && !value) {
        errors.push('该字段为必填项');
    }

    if (rules.type === 'string' && typeof value !== 'string') {
        errors.push('必须是字符串类型');
    }

    if (rules.enum && !rules.enum.includes(value)) {
        errors.push(`必须是以下值之一: ${rules.enum.join(', ')}`);
    }

    return errors;
}
