import axios from 'axios'

const API_BASE = import.meta.env.VITE_API_URL || 'http://localhost:8080'

// Session ID management
let currentSessionId: string | null = null

const api = axios.create({
  baseURL: API_BASE,
  headers: {
    'Content-Type': 'application/json'
  }
})

// Request interceptor: attach session ID header
api.interceptors.request.use((config) => {
  if (currentSessionId) {
    config.headers['X-Session-Id'] = currentSessionId
  }
  return config
})

export const GameAPI = {
  // 开始游戏
  async startGame() {
    const response = await api.post('/game/start')
    // Save session ID from server
    if (response.data.sessionId) {
      currentSessionId = response.data.sessionId
    }
    return response.data
  },

  // 重新开始游戏
  async restartGame() {
    const response = await api.post('/game/restart')
    if (response.data.sessionId) {
      currentSessionId = response.data.sessionId
    }
    return response.data
  },

  // 玩家移动
  async movePlayer(direction: string) {
    const response = await api.post('/game/player/move', { direction })
    return response.data
  },

  // 开始战斗
  async startCombat() {
    const response = await api.post('/game/combat/start')
    return response.data
  },

  // 获取地图渲染数据
  async getMapRender() {
    const response = await api.get('/game/map/render')
    return response.data
  },

  // 获取玩家状态
  async getPlayerStatus() {
    const response = await api.get('/game/player/status')
    return response.data
  }
}

export default GameAPI
