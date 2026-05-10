<template>
  <div class="card map-card h-100">
    <div class="card-header d-flex align-items-center justify-content-between">
      <div class="d-flex align-items-center">
        <i class="bi bi-map me-2"></i>
        <h5 class="mb-0">地牢地图</h5>
      </div>
      <div class="map-legend">
        <span class="legend-item">
          <span class="legend-symbol player">@</span> 玩家
        </span>
        <span class="legend-item">
          <span class="legend-symbol enemy">E</span> 敌人
        </span>
        <span class="legend-item">
          <span class="legend-symbol trail">*</span> 足迹
        </span>
        <span class="legend-item">
          <span class="legend-symbol wall">#</span> 墙壁
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
              {{ getCellContent(x, y) }}
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
  
  // 基础颜色 #c4a25a (196, 162, 90)
  // 最新: 原色, 最旧: 淡化版本
  const r = Math.round(196 + (245 - 196) * (1 - freshness))
  const g = Math.round(162 + (226 - 162) * (1 - freshness))
  const b = Math.round(90 + (195 - 90) * (1 - freshness))
  
  return {
    backgroundColor: `rgb(${r}, ${g}, ${b})`,
    opacity: 0.3 + freshness * 0.7 // 最旧30%透明度，最新100%
  }
}

const getCellContent = (x, y) => {
  if (!isVisible(x, y)) return '?'
  if (isPlayerAt(x, y)) return '@'
  if (getEnemyAt(x, y)) return 'E'
  if (getTrailIndex(x, y) !== -1) return '*'
  if (props.mapData[y][x] === 1) return '#'
  return '.'
}
</script>

<style scoped>
.map-card {
  background: #fcefd7;
  border: 1px solid #7da8b1;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(125, 168, 177, 0.2);
}

.card-header {
  background: #7da8b1;
  color: #f5e2c3;
  border-bottom: 1px solid #7da8b1;
  border-radius: 7px 7px 0 0;
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
  width: 18px;
  height: 18px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 3px;
  font-weight: bold;
  font-size: 11px;
}

.legend-symbol.player {
  background: #9c8d6d;
  color: #f0e6d2;
}

.legend-symbol.enemy {
  background: #c47e5a;
  color: #f0e6d2;
}

.legend-symbol.trail {
  background: #c4a25a;
  color: #f0e6d2;
}

.legend-symbol.wall {
  background: #7da8b1;
  color: #f0e6d2;
}

.map-container {
  display: flex;
  align-items: center;
  justify-content: center;
  overflow: auto;
  padding: 10px;
  background: #f5e2c3;
  border-radius: 4px;
}

/* 使用原来的 map-grid 样式 */
.map-grid {
  display: grid;
  grid-template-columns: repeat(20, 20px);
  grid-auto-rows: 20px;
  gap: 0;
  margin: 0 auto;
  background: #7da8b1;
  padding: 10px;
  border-radius: 4px;
  box-shadow: 0 4px 6px rgba(92, 75, 60, 0.1);
  width: 420px;
  height: 420px;
  overflow: auto;
}

.map-row {
  display: contents;
}

.map-cell {
  width: 20px;
  height: 20px;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 12px;
  font-weight: bold;
  border-radius: 0;
  transition: all 0.2s ease;
  margin: 0;
  padding: 0;
  line-height: 1;
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
  color: #5a4a42;
}
</style>
