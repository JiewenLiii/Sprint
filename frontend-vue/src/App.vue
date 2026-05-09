<template>
  <div class="game-container">
    <header class="game-header">
      <h1>⚔️ 地牢探险 ⚔️</h1>
      <p class="subtitle">探索地牢，消灭所有敌人！</p>
    </header>

    <main class="game-main">
      <div class="left-panel">
        <PlayerStatus
          :player="player"
          :enemy-count="aliveEnemies"
          :total-enemies="totalEnemies"
        />
      </div>

      <div class="center-panel">
        <GameMap
          v-if="mapData.length > 0"
          :map-data="mapData"
          :fog="fog"
          :player-position="player.position"
          :enemies="visibleEnemies"
          :player-trail="playerTrail"
        />
        <div v-else class="loading">
          <p>加载中...</p>
        </div>

        <div class="game-message" v-if="gameMessage">
          <p :class="gameResult">{{ gameMessage }}</p>
        </div>
      </div>

      <div class="right-panel">
        <GameLog :logs="logs" />
        <ControlPanel
          :disabled="gameOver"
          @move="handleMove"
          @restart="handleRestart"
        />
      </div>
    </main>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import GameMap from './components/GameMap.vue'
import PlayerStatus from './components/PlayerStatus.vue'
import ControlPanel from './components/ControlPanel.vue'
import GameLog from './components/GameLog.vue'
import GameAPI from './api/gameApi'

// Types
interface Enemy {
  id: string
  position: number[]
  hp: number
  attack: number
  colorIndex: number
}

interface Player {
  id: string
  position: number[]
  hp: number
  attack: number
  isAlive: boolean
  maxHp?: number
}

interface LogEntry {
  time: string
  message: string
  type: 'info' | 'success' | 'error' | 'warning'
}

// State
const player = ref<Player>({
  id: 'player_001',
  position: [10, 10],
  hp: 20,
  attack: 5,
  isAlive: true,
  maxHp: 20
})

const mapData = ref<number[][]>([])
const fog = ref<boolean[][]>([])
const visibleEnemies = ref<Enemy[]>([])
const playerTrail = ref<number[][]>([])
const logs = ref<LogEntry[]>([])
const gameOver = ref(false)
const gameResult = ref('')
const gameMessage = ref('')
const totalEnemies = ref(4)

// Computed
const aliveEnemies = computed(() => visibleEnemies.value.length)

// Methods
const addLog = (message: string, type: LogEntry['type'] = 'info') => {
  const now = new Date()
  const time = now.toLocaleTimeString()
  logs.value.push({ time, message, type })
  if (logs.value.length > 50) {
    logs.value.shift()
  }
}

const startGame = async () => {
  try {
    addLog('正在初始化游戏...', 'info')
    const data = await GameAPI.startGame()

    player.value = data.player
    mapData.value = data.mapRender.mapData
    fog.value = data.mapRender.fog
    visibleEnemies.value = data.mapRender.enemies
    playerTrail.value = data.mapRender.playerTrail || []
    totalEnemies.value = 4

    gameOver.value = false
    gameMessage.value = ''

    addLog('游戏开始！', 'success')
  } catch (error: any) {
    addLog(`游戏初始化失败: ${error.message}`, 'error')
    console.error('Failed to start game:', error)
  }
}

const handleMove = async (direction: string) => {
  if (gameOver.value) return

  try {
    addLog(`移动: ${direction}`, 'info')
    const data = await GameAPI.movePlayer(direction)

    player.value = data.playerStatus
    mapData.value = data.mapRender.mapData
    fog.value = data.mapRender.fog
    visibleEnemies.value = data.mapRender.enemies
    playerTrail.value = data.mapRender.playerTrail || []

    addLog(`移动成功 → (${player.value.position[0]}, ${player.value.position[1]})`, 'success')

    // Check for combat
    if (data.combat) {
      handleCombatResult(data.combat)
    }

    // Check game over
    if (!player.value.isAlive) {
      gameOver.value = true
      gameResult.value = 'defeat'
      gameMessage.value = '💀 游戏结束 - 你被击败了！'
      addLog('游戏结束 - 你被击败了！', 'error')
    }
  } catch (error: any) {
    addLog(`移动失败: ${error.response?.data?.detail || error.message}`, 'error')
  }
}

const handleCombatResult = (combat: any) => {
  addLog(`⚔️ 遭遇敌人 ${combat.enemy.id}！`, 'warning')

  combat.log?.forEach((log: string) => {
    addLog(log, combat.result === 'win' ? 'success' : 'error')
  })

  if (combat.result === 'win') {
    addLog(`✅ 击败了 ${combat.enemy.id}！`, 'success')

    // Check victory
    if (visibleEnemies.value.length === 0) {
      gameOver.value = true
      gameResult.value = 'victory'
      gameMessage.value = '🎉 胜利！你消灭了所有敌人！'
      addLog('🎉 胜利！你消灭了所有敌人！', 'success')
    }
  } else {
    addLog(`❌ 被 ${combat.enemy.id} 击败！`, 'error')
  }
}

const handleRestart = async () => {
  try {
    addLog('正在重新开始游戏...', 'info')
    const data = await GameAPI.restartGame()

    player.value = data.player
    mapData.value = data.mapRender.mapData
    fog.value = data.mapRender.fog
    visibleEnemies.value = data.mapRender.enemies
    playerTrail.value = data.mapRender.playerTrail || []

    gameOver.value = false
    gameResult.value = ''
    gameMessage.value = ''

    addLog('游戏重新开始！', 'success')
  } catch (error: any) {
    addLog(`重新开始失败: ${error.message}`, 'error')
  }
}

// Keyboard controls
const handleKeyDown = (event: KeyboardEvent) => {
  if (gameOver.value) return

  const keyMap: Record<string, string> = {
    'w': 'up',
    'W': 'up',
    'ArrowUp': 'up',
    's': 'down',
    'S': 'down',
    'ArrowDown': 'down',
    'a': 'left',
    'A': 'left',
    'ArrowLeft': 'left',
    'd': 'right',
    'D': 'right',
    'ArrowRight': 'right'
  }

  const direction = keyMap[event.key]
  if (direction) {
    event.preventDefault()
    handleMove(direction)
  }
}

// Lifecycle
onMounted(() => {
  startGame()
  window.addEventListener('keydown', handleKeyDown)
})
</script>

<style>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  background: #0f0f1a;
  color: #fff;
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  min-height: 100vh;
}

#app {
  min-height: 100vh;
}
</style>

<style scoped>
.game-container {
  max-width: 1400px;
  margin: 0 auto;
  padding: 20px;
  min-height: 100vh;
}

.game-header {
  text-align: center;
  margin-bottom: 20px;
}

.game-header h1 {
  font-size: 2.5rem;
  color: #f39c12;
  text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.5);
}

.subtitle {
  color: #888;
  margin-top: 5px;
}

.game-main {
  display: grid;
  grid-template-columns: 200px 1fr 300px;
  gap: 20px;
  min-height: 600px;
}

.left-panel, .right-panel {
  display: flex;
  flex-direction: column;
  gap: 15px;
}

.center-panel {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 15px;
}

.loading {
  display: flex;
  align-items: center;
  justify-content: center;
  height: 500px;
  background: #1a1a2e;
  border-radius: 8px;
  color: #888;
}

.game-message {
  padding: 20px;
  border-radius: 8px;
  text-align: center;
  font-size: 1.5rem;
  font-weight: bold;
}

.game-message .victory {
  color: #2ecc71;
  background: rgba(46, 204, 113, 0.1);
}

.game-message .defeat {
  color: #e74c3c;
  background: rgba(231, 76, 60, 0.1);
}

@media (max-width: 1200px) {
  .game-main {
    grid-template-columns: 1fr;
  }

  .left-panel, .right-panel {
    flex-direction: row;
    flex-wrap: wrap;
    justify-content: center;
  }
}
</style>
