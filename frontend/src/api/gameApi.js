// API 配置 - 支持环境变量配置
const API_BASE = import.meta.env.VITE_API_URL || ''

// Session ID 管理
let currentSessionId = null

// 通用请求函数
async function apiRequest(url, options = {}) {
  const headers = {
    'Content-Type': 'application/json',
    ...options.headers
  }
  
  // 添加 Session ID
  if (currentSessionId) {
    headers['X-Session-Id'] = currentSessionId
  }
  
  const response = await fetch(`${API_BASE}${url}`, {
    ...options,
    headers
  })
  
  if (!response.ok) {
    const error = await response.json().catch(() => ({ detail: '请求失败' }))
    throw new Error(error.detail || `HTTP ${response.status}`)
  }
  
  const data = await response.json()
  
  // 保存服务器返回的 Session ID
  if (data.sessionId) {
    currentSessionId = data.sessionId
  }
  
  return data
}

export const GameAPI = {
  // 开始游戏
  async startGame(difficulty = 'easy') {
    return apiRequest('/game/start', {
      method: 'POST',
      body: JSON.stringify({ difficulty })
    })
  },

  // 重新开始游戏
  async restartGame(difficulty = 'easy') {
    return apiRequest('/game/restart', {
      method: 'POST',
      body: JSON.stringify({ difficulty })
    })
  },
  
  // 玩家移动
  async movePlayer(direction) {
    return apiRequest('/game/player/move', {
      method: 'POST',
      body: JSON.stringify({ direction })
    })
  },
  
  // 开始战斗
  async startCombat() {
    return apiRequest('/game/combat/start', { method: 'POST' })
  },
  
  // 获取地图渲染数据
  async getMapRender() {
    return apiRequest('/game/map/render')
  },
  
  // 获取玩家状态
  async getPlayerStatus() {
    return apiRequest('/game/player/status')
  },
  
  // 获取当前 Session ID
  getSessionId() {
    return currentSessionId
  }
}

export default GameAPI
