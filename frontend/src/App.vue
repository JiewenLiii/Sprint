<template>
  <div class="game-wrapper">
    <!-- 标题区域 -->
    <header class="game-header text-center py-3">
      <h1 class="game-title">地牢探险</h1>
      <p class="game-subtitle">探索神秘地牢，消灭所有敌人！</p>
    </header>

    <!-- 游戏主体区域 -->
    <main class="game-main">
      <div class="game-content">
        <!-- 左侧：玩家状态 -->
        <div class="col-left">
          <PlayerStatus
            :player="player"
            :alive-enemies="aliveEnemies"
            :total-enemies="totalEnemies"
          />
        </div>

        <!-- 中间：地图区域 -->
        <div class="col-center">
          <GameMap
            :map-data="mapData"
            :fog="fog"
            :player-position="player.position"
            :enemies="visibleEnemies"
            :player-trail="playerTrail"
          />
        </div>

        <!-- 右侧：日志 -->
        <div class="col-right">
          <GameLog
            :logs="logs"
            :disabled="gameOver || combatVisible"
            @restart="handleRestart"
          />
        </div>
      </div>
    </main>

    <!-- 难度选择弹窗 -->
    <DifficultyModal
      :visible="difficultyVisible"
      @select="handleDifficultySelect"
    />

    <!-- 战斗弹窗 -->
    <CombatModal
      :visible="combatVisible"
      :player="combatPlayer"
      :enemy="combatEnemy"
      :combat-log="combatLog"
      :result="combatResult"
      @close="closeCombat"
    />

    <!-- 游戏结束弹窗 -->
    <div v-if="gameOver" class="game-over-overlay">
      <div class="game-over-modal">
        <h2 :class="gameResult === 'victory' ? 'text-success' : 'text-danger'">
          {{ gameResult === 'victory' ? '胜利！' : '游戏结束' }}
        </h2>
        <p>{{ gameMessage }}</p>
        <button class="btn btn-warning" @click="handleRestart">
          重新开始
        </button>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted, onUnmounted } from 'vue'
import PlayerStatus from './components/PlayerStatus.vue'
import GameMap from './components/GameMap.vue'
import GameLog from './components/GameLog.vue'
import CombatModal from './components/CombatModal.vue'
import DifficultyModal from './components/DifficultyModal.vue'
import GameAPI from './api/gameApi.js'

// 游戏状态
const player = ref({
  id: 'player_001',
  position: [10, 10],
  hp: 20,
  maxHp: 20,
  attack: 5,
  isAlive: true
})

const mapData = ref([])
const fog = ref([])
const visibleEnemies = ref([])
const playerTrail = ref([])
const logs = ref([])
const gameOver = ref(false)
const gameResult = ref('')
const gameMessage = ref('')
const totalEnemies = ref(4)
const aliveEnemiesCount = ref(4)

// 难度选择状态
const difficultyVisible = ref(true)
const difficulty = ref('easy')

// 战斗弹窗状态
const combatVisible = ref(false)
const combatPlayer = ref({})
const combatEnemy = ref({})
const combatLog = ref([])
const combatResult = ref('')

const aliveEnemies = computed(() => aliveEnemiesCount.value)

// 添加日志
const addLog = (message, type = 'info') => {
  const now = new Date()
  const time = now.toLocaleTimeString('zh-CN', { hour12: false })
  logs.value.push({ time, message, type })
  if (logs.value.length > 50) {
    logs.value.shift()
  }
}

// 处理难度选择
const handleDifficultySelect = async (selectedDifficulty) => {
  difficulty.value = selectedDifficulty
  difficultyVisible.value = false
  await startGame()
}

// 开始游戏
const startGame = async () => {
  try {
    addLog('正在初始化游戏...', 'info')
    const data = await GameAPI.startGame(difficulty.value)

    player.value = data.player
    mapData.value = data.mapRender.mapData
    fog.value = data.mapRender.fog
    visibleEnemies.value = data.mapRender.enemies
    playerTrail.value = data.mapRender.playerTrail || []
    totalEnemies.value = data.aliveEnemiesCount
    aliveEnemiesCount.value = data.aliveEnemiesCount

    gameOver.value = false
    gameResult.value = ''
    gameMessage.value = ''

    addLog(`游戏开始！难度: ${difficulty.value === 'easy' ? '简单' : '困难'}`, 'success')
  } catch (error) {
    addLog(`游戏初始化失败: ${error.message}`, 'error')
    console.error('Failed to start game:', error)
  }
}

// 处理移动
const handleMove = async (direction) => {
  if (gameOver.value || combatVisible.value) return

  try {
    addLog(`移动: ${direction}`, 'info')
    const preMovePlayer = { ...player.value }
    const data = await GameAPI.movePlayer(direction)

    player.value = data.playerStatus
    mapData.value = data.mapRender.mapData
    fog.value = data.mapRender.fog
    visibleEnemies.value = data.mapRender.enemies
    playerTrail.value = data.mapRender.playerTrail || []
    aliveEnemiesCount.value = data.aliveEnemiesCount

    addLog(`移动成功 → (${player.value.position[0]}, ${player.value.position[1]})`, 'success')

    if (data.combat) {
      openCombat(data.combat, preMovePlayer)
    }

    if (!player.value.isAlive) {
      gameOver.value = true
      gameResult.value = 'defeat'
      gameMessage.value = '你被击败了！'
      addLog('游戏结束 - 你被击败了！', 'error')
    }
  } catch (error) {
    addLog(`移动失败: ${error.message}`, 'error')
  }
}

// 打开战斗弹窗
const openCombat = (combat, preMovePlayer) => {
  combatPlayer.value = { ...preMovePlayer }
  combatEnemy.value = { ...combat.enemy }
  combatResult.value = ''
  combatLog.value = []
  combatVisible.value = true

  if (combat.log) {
    combat.log.forEach((log, index) => {
      setTimeout(() => {
        combatLog.value.push({
          round: `Round ${index + 1}`,
          message: log,
          type: log.includes('胜利') || log.includes('win') ? 'win' :
                log.includes('失败') || log.includes('lose') ? 'lose' : 'damage'
        })

        if (log.includes('attacks')) {
          const damage = parseInt(log.match(/for (\d+) damage/)?.[1] || 0)
          const match = log.match(/Round \d+: (\w+) attacks (\w+) for (\d+) damage/)
          if (match) {
            const defender = match[2]
            if (defender === combat.enemy.id) {
              combatEnemy.value.hp = Math.max(0, combatEnemy.value.hp - damage)
            } else if (defender === preMovePlayer.id) {
              combatPlayer.value.hp = Math.max(0, combatPlayer.value.hp - damage)
            }
          }
        }
      }, index * 800)
    })

    setTimeout(() => {
      combatResult.value = combat.result
      combatEnemy.value.hp = combat.finalEnemyHp || 0
      combatPlayer.value.hp = player.value.hp
    }, combat.log.length * 800 + 500)
  }
}

// 关闭战斗弹窗
const closeCombat = () => {
  combatVisible.value = false
  if (combatResult.value === 'win' && aliveEnemiesCount.value === 0) {
    gameOver.value = true
    gameResult.value = 'victory'
    gameMessage.value = '你消灭了所有敌人！'
    addLog('胜利！你消灭了所有敌人！', 'success')
  }
}

// 重新开始
const handleRestart = async () => {
  try {
    addLog('正在重新开始游戏...', 'info')
    const data = await GameAPI.restartGame(difficulty.value)

    player.value = data.player
    mapData.value = data.mapRender.mapData
    fog.value = data.mapRender.fog
    visibleEnemies.value = data.mapRender.enemies
    playerTrail.value = data.mapRender.playerTrail || []
    totalEnemies.value = data.aliveEnemiesCount
    aliveEnemiesCount.value = data.aliveEnemiesCount

    gameOver.value = false
    gameResult.value = ''
    gameMessage.value = ''
    combatVisible.value = false
    difficultyVisible.value = true

    addLog('游戏重新开始！', 'success')
  } catch (error) {
    addLog(`重新开始失败: ${error.message}`, 'error')
  }
}

// 键盘控制
const handleKeyDown = (event) => {
  if (gameOver.value || combatVisible.value) return

  const keyMap = {
    'w': 'up', 'W': 'up', 'ArrowUp': 'up',
    's': 'down', 'S': 'down', 'ArrowDown': 'down',
    'a': 'left', 'A': 'left', 'ArrowLeft': 'left',
    'd': 'right', 'D': 'right', 'ArrowRight': 'right'
  }

  const direction = keyMap[event.key]
  if (direction) {
    event.preventDefault()
    handleMove(direction)
  }
}

onMounted(() => {
  window.addEventListener('keydown', handleKeyDown)
})

onUnmounted(() => {
  window.removeEventListener('keydown', handleKeyDown)
})
</script>

<style>
/* 全局样式 */
body {
  background-color: #fcefd7 !important;
  color: #5a4a42;
  height: 100vh;
  margin: 0;
  padding: 0;
  overflow: hidden;
}

#app {
  height: 100vh;
  overflow: hidden;
}
</style>

<style scoped>
/* 整体包裹 */
.game-wrapper {
  height: 100vh;
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

/* 标题栏 */
.game-header {
  background: #7da8b1;
  border-bottom: 2px solid #5a8a94;
  box-shadow: 0 2px 4px rgba(92, 75, 60, 0.1);
  flex: 0 0 auto;
  padding: 8px 0;
}

.game-title {
  font-size: 2rem;
  font-weight: bold;
  color: #f5e2c3;
  margin: 0;
}

.game-subtitle {
  color: #f5e2c3;
  font-size: 1rem;
  margin: 0;
  opacity: 0.9;
}

/* 主内容区 */
.game-main {
  flex: 1 1 0%;
  min-height: 0;
  overflow: hidden;
  padding: 10px;
  display: flex;
  flex-direction: column;
}

/* 三列容器 — 纯 flex，不用 Bootstrap grid */
.game-content {
  display: flex;
  gap: 10px;
  flex: 1;
  min-height: 0;
  overflow: hidden;
}

/* 列 */
.col-left {
  flex: 0 0 18%;
  min-width: 0;
  min-height: 0;
  overflow: hidden;
  display: flex;
  flex-direction: column;
}

.col-center {
  flex: 1 1 0%;
  min-width: 0;
  min-height: 0;
  overflow: hidden;
  display: flex;
  flex-direction: column;
}

.col-right {
  flex: 0 0 32%;
  min-width: 0;
  min-height: 0;
  overflow: hidden;
  display: flex;
  flex-direction: column;
}

/* 游戏结束弹窗 */
.game-over-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(92, 75, 60, 0.7);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 1000;
}

.game-over-modal {
  background: #fcefd7;
  border: 2px solid #7da8b1;
  border-radius: 15px;
  padding: 30px;
  text-align: center;
  box-shadow: 0 10px 30px rgba(92, 75, 60, 0.3);
  max-width: 350px;
  width: 90%;
}

.game-over-modal h2 {
  font-size: 1.8rem;
  margin-bottom: 15px;
}

.game-over-modal p {
  color: #5a4a42;
  margin-bottom: 20px;
  font-size: 1rem;
}
</style>
