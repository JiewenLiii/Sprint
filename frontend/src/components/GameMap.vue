<template>
  <div class="card map-card">
    <div class="card-header d-flex align-items-center justify-content-between">
      <div class="d-flex align-items-center">
        <i class="bi bi-map me-2"></i>
        <h5 class="mb-0">地牢地图</h5>
      </div>
      <div class="map-legend">
        <span class="legend-item">
          <span class="legend-symbol player-icon">🧙</span> 玩家
        </span>
        <span class="legend-item">
          <span class="legend-symbol enemy-icon">👹</span> 敌人
        </span>
        <span class="legend-item">
          <span class="legend-symbol trail-icon">👣</span> 足迹
        </span>
        <span class="legend-item">
          <span class="legend-symbol wall-icon">🧱</span> 墙壁
        </span>
      </div>
    </div>
    <div class="card-body d-flex flex-column">
      <div class="map-container flex-grow-1">
        <div v-if="!mapData.length" class="text-center py-5">
          <div class="spinner-border text-primary" role="status">
            <span class="visually-hidden">加载中...</span>
          </div>
          <p class="mt-2 text-muted">正在加载地图...</p>
        </div>
        <div v-else class="map-grid">
          <div
            v-for="(row, y) in mapData"
            :key="y"
            class="map-row"
          >
            <div
              v-for="(cell, x) in row"
              :key="`${x}-${y}`"
              class="map-cell"
              :class="getCellClass(x, y)"
              :style="getCellStyle(x, y)"
            >
              <div class="cell-content">
                <span v-if="isPlayerAt(x, y)" class="player-sprite">🧙</span>
                <span v-else-if="getEnemyAt(x, y)" class="enemy-sprite">👹</span>
                <span v-else-if="getTrailIndex(x, y) !== -1" class="trail-icon">👣</span>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
const props = defineProps({
  mapData: {
    type: Array,
    default: () => []
  },
  fog: {
    type: Array,
    default: () => []
  },
  playerPosition: {
    type: Array,
    default: () => [10, 10]
  },
  enemies: {
    type: Array,
    default: () => []
  },
  playerTrail: {
    type: Array,
    default: () => []
  }
})

const isPlayerAt = (x, y) => {
  return props.playerPosition[0] === x && props.playerPosition[1] === y
}

const getEnemyAt = (x, y) => {
  return props.enemies.find(e => e.position[0] === x && e.position[1] === y)
}

const getTrailIndex = (x, y) => {
  return props.playerTrail.findIndex(t => t[0] === x && t[1] === y)
}

const isVisible = (x, y) => {
  if (!props.fog || !props.fog[y]) return true
  return props.fog[y][x]
}

const getCellClass = (x, y) => {
  const classes = []
  
  if (!isVisible(x, y)) {
    classes.push('hidden')
    return classes
  }
  
  if (isPlayerAt(x, y)) {
    classes.push('player')
  } else if (getEnemyAt(x, y)) {
    classes.push('enemy')
  } else if (getTrailIndex(x, y) !== -1) {
    classes.push('trail')
  } else if (props.mapData[y][x] === 1) {
    classes.push('wall')
  } else {
    classes.push('floor')
  }
  
  return classes
}

// 计算足迹颜色渐变 - 最新深，最旧浅
const getCellStyle = (x, y) => {
  const trailIndex = getTrailIndex(x, y)
  if (trailIndex === -1) return {}

  // trailIndex: 0 = 最旧, length-1 = 最新
  const total = props.playerTrail.length
  const freshness = (trailIndex + 1) / total // 0.2 ~ 1.0

  // 基础颜色 #c4a25a (196, 162, 90) 渐变到地板色 #f5e2c3 (245, 226, 195)
  const r = Math.round(196 + (245 - 196) * (1 - freshness))
  const g = Math.round(162 + (226 - 162) * (1 - freshness))
  const b = Math.round(90 + (195 - 90) * (1 - freshness))

  return {
    backgroundColor: `rgb(${r}, ${g}, ${b})`,
    opacity: 0.3 + freshness * 0.7
  }
}
</script>

<style scoped>
.map-card {
  background: #fcefd7;
  border: 1px solid #7da8b1;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(125, 168, 177, 0.2);
  flex: 1;
  min-height: 0;
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.card-header {
  background: #7da8b1;
  color: #f5e2c3;
  border-bottom: 1px solid #7da8b1;
  border-radius: 7px 7px 0 0;
  flex-shrink: 0;
}

.card-body {
  flex: 1;
  min-height: 0;
  overflow: hidden;
}

.map-legend {
  display: flex;
  gap: 10px;
  font-size: 0.8rem;
}

.legend-item {
  display: flex;
  align-items: center;
  gap: 4px;
  color: #f5e2c3;
}

.legend-symbol {
  width: 24px;
  height: 24px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 6px;
  font-size: 16px;
  background: rgba(0, 0, 0, 0.1);
}

.legend-symbol.player-icon {
  background: #9c8d6d;
}

.legend-symbol.enemy-icon {
  background: #c47e5a;
}

.legend-symbol.trail-icon {
  background: #c4a25a;
}

.legend-symbol.wall-icon {
  background: #7da8b1;
}

.map-container {
  display: flex;
  align-items: center;
  justify-content: center;
  overflow: auto;
  padding: 10px;
  background: #f5e2c3;
  border-radius: 4px;
  flex: 1;
  min-height: 0;
}

/* 使用原来的 map-grid 样式 */
.map-grid {
  display: grid;
  grid-template-columns: repeat(20, 1fr);
  grid-auto-rows: 1fr;
  gap: 0;
  margin: auto;
  background: #7da8b1;
  padding: 10px;
  border-radius: 4px;
  box-shadow: 0 4px 6px rgba(92, 75, 60, 0.1);
  max-width: 100%;
  max-height: 100%;
  width: 600px;
  height: 600px;
  overflow: hidden;
}

.map-row {
  display: contents;
}

.map-cell {
  width: 100%;
  height: 100%;
  aspect-ratio: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.2s ease;
  position: relative;
}

.cell-content {
  width: 100%;
  height: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 18px;
  z-index: 10;
}

/* 使用原来的颜色方案 */
.map-cell.hidden {
  background: #5a7a82;
  color: #3a5a62;
}

.map-cell.floor {
  background: #f5e2c3;
  color: #666;
}

.map-cell.wall {
  background: #7da8b1;
  color: #f0e6d2;
}

.map-cell.player {
  background: #9c8d6d;
  color: #f0e6d2;
  box-shadow: 0 0 5px rgba(156, 141, 109, 0.8);
  z-index: 2;
}

.map-cell.enemy {
  background: #c47e5a;
  color: #f0e6d2;
  box-shadow: 0 0 5px rgba(196, 126, 90, 0.8);
  z-index: 1;
}

/* 足迹样式由 getCellStyle 动态计算 */
.map-cell.trail {
  background: #f5e2c3;
  color: #5a4a42;
}

.player-sprite {
  animation: playerBounce 1s ease-in-out infinite;
  display: block;
}

.enemy-sprite {
  animation: enemyBreath 1.5s ease-in-out infinite;
  display: block;
}

@keyframes playerBounce {
  0%, 100% { transform: translateY(0); }
  50% { transform: translateY(-2px); }
}

@keyframes enemyBreath {
  0%, 100% { transform: scale(1); }
  50% { transform: scale(1.05); }
}

.trail-icon {
  font-size: 14px;
  opacity: 0.6;
}
</style>
