<template>
  <div class="game-wrapper">
    <!-- 标题区域 -->
    <header class="game-header text-center py-3">
      <div class="container">
        <h1 class="game-title">
          地牢探险
        </h1>
        <p class="game-subtitle">探索神秘地牢，消灭所有敌人！</p>
      </div>
    </header>

    <!-- 游戏主体区域 -->
    <main class="game-main">
      <div class="container">
        <div class="row g-3">
          <!-- 左侧：玩家状态 -->
          <div class="col-md-2">
            <PlayerStatus 
              :player="player" 
              :alive-enemies="aliveEnemies" 
              :total-enemies="totalEnemies" 
            />
          </div>

          <!-- 中间：地图区域 -->
          <div class="col-md-6">
            <GameMap 
              :map-data="mapData"
              :fog="fog"
              :player-position="player.position"
              :enemies="visibleEnemies"
              :player-trail="playerTrail"
            />
          </div>

          <!-- 右侧：日志和控制 -->
          <div class="col-md-4">
            <div class="log-direction-container">
              <div class="log-section">
                <GameLog :logs="logs" />
              </div>
              <div class="direction-section">
                <ControlPanel 
                  :disabled="gameOver || combatVisible" 
                  @move="handleMove" 
                  @restart="handleRestart" 
                />
              </div>
            </div>
          </div>
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
import ControlPanel from './components/ControlPanel.vue'
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

// 难度选择状态
const difficultyVisible = ref(true)
const difficulty = ref('easy')

// 战斗弹窗状态
const combatVisible = ref(false)
const combatPlayer = ref({})
const combatEnemy = ref({})
const combatLog = ref([])
const combatResult = ref('')

const aliveEnemies = computed(() => visibleEnemies.value.length)

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
    const data = await GameAPI.startGame()
    
    player.value = data.player
    mapData.value = data.mapRender.mapData
    fog.value = data.mapRender.fog
    visibleEnemies.value = data.mapRender.enemies
    playerTrail.value = data.mapRender.playerTrail || []
    totalEnemies.value = data.mapRender.enemies.length
    
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
    const data = await GameAPI.movePlayer(direction)
    
    // 更新玩家状态（后端返回200即表示成功）
    player.value = data.playerStatus
    mapData.value = data.mapRender.mapData
    fog.value = data.mapRender.fog
    visibleEnemies.value = data.mapRender.enemies
    playerTrail.value = data.mapRender.playerTrail || []
    
    addLog(`移动成功 → (${player.value.position[0]}, ${player.value.position[1]})`, 'success')
    
    // 处理战斗
    if (data.combat) {
      openCombat(data.combat)
    }
    
    // 检查游戏结束
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
const openCombat = (combat) => {
  combatPlayer.value = { ...player.value }
  combatEnemy.value = combat.enemy
  combatResult.value = ''
  combatLog.value = []
  combatVisible.value = true
  
  addLog(`遭遇敌人 ${combat.enemy.id}！`, 'combat')
  
  // 模拟战斗过程
  if (combat.log) {
    let round = 1
    combat.log.forEach((log, index) => {
      setTimeout(() => {
        combatLog.value.push({
          round: `Round ${round}`,
          message: log,
          type: log.includes('胜利') || log.includes('win') ? 'win' : 
                log.includes('失败') || log.includes('lose') ? 'lose' : 'damage'
        })
        
        // 更新血条（简化模拟）
        if (log.includes('attacks')) {
          const damage = parseInt(log.match(/for (\d+) damage/)?.[1] || 0)
          if (log.includes(combat.enemy.id)) {
            combatPlayer.value.hp = Math.max(0, combatPlayer.value.hp - damage)
          } else {
            combatEnemy.value.hp = Math.max(0, combatEnemy.value.hp - damage)
          }
        }
        
        round++
      }, index * 800)
    })
    
    // 战斗结束
    setTimeout(() => {
      combatResult.value = combat.result
      if (combat.result === 'win') {
        addLog(`击败了 ${combat.enemy.id}！`, 'success')
      } else {
        addLog(`被 ${combat.enemy.id} 击败！`, 'error')
      }
    }, combat.log.length * 800 + 500)
  }
}

// 关闭战斗弹窗
const closeCombat = () => {
  combatVisible.value = false
  
  // 检查胜利
  if (combatResult.value === 'win' && visibleEnemies.value.length === 0) {
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
    const data = await GameAPI.restartGame()
    
    player.value = data.player
    mapData.value = data.mapRender.mapData
    fog.value = data.mapRender.fog
    visibleEnemies.value = data.mapRender.enemies
    playerTrail.value = data.mapRender.playerTrail || []
    
    gameOver.value = false
    gameResult.value = ''
    gameMessage.value = ''
    combatVisible.value = false
    difficultyVisible.value = true // 重新选择难度
    
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

// 生命周期
onMounted(() => {
  // 等待用户选择难度后再开始游戏
  window.addEventListener('keydown', handleKeyDown)
})

onUnmounted(() => {
  window.removeEventListener('keydown', handleKeyDown)
})
</script>

<style>
/* 全局样式 - 使用原来的暖色调 */
body {
  background-color: #fcefd7 !important;
  color: #5a4a42;
  min-height: 100vh;
  margin: 0;
  padding: 10px;
  overflow: hidden;
}

#app {
  min-height: 100vh;
}
</style>

<style scoped>
.game-wrapper {
  min-height: 100vh;
}

.game-header {
  background: #7da8b1;
  border-bottom: 2px solid #5a8a94;
  box-shadow: 0 2px 4px rgba(92, 75, 60, 0.1);
}

.game-title {
  font-size: 2rem;
  font-weight: bold;
  color: #f5e2c3;
  margin-bottom: 0.3rem;
}

.game-subtitle {
  color: #f5e2c3;
  font-size: 1rem;
  margin-bottom: 0;
  opacity: 0.9;
}

.game-main {
  padding: 15px 0;
}

.container {
  background-color: #fcefd7;
  border-radius: 10px;
  padding: 15px;
  box-shadow: 0 4px 8px rgba(92, 75, 60, 0.15);
  max-width: 98vw;
  margin: 0 auto;
  max-height: 98vh;
  overflow: hidden;
}

.row {
  margin: 0;
  height: 600px;
}

/* 三列布局 */
.col-md-2 {
  flex: 0 0 18%;
  max-width: 18%;
}

.col-md-6 {
  flex: 0 0 50%;
  max-width: 50%;
}

.col-md-4 {
  flex: 0 0 32%;
  max-width: 32%;
}

/* 日志和方向键容器 */
.log-direction-container {
  display: flex;
  flex-direction: column;
  height: 100%;
}

.log-section {
  flex: 7;
  margin-bottom: 10px;
}

.direction-section {
  flex: 3;
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
