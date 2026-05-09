<template>
  <div class="game-map">
    <div class="map-grid">
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
        >
          {{ getCellContent(x, y) }}
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'

interface Enemy {
  id: string
  position: number[]
  hp: number
  attack: number
  colorIndex: number
}

interface Props {
  mapData: number[][]
  fog: boolean[][]
  playerPosition: number[]
  enemies: Enemy[]
  playerTrail: number[][]
}

const props = defineProps<Props>()

const isPlayerAt = (x: number, y: number) => {
  return props.playerPosition[0] === x && props.playerPosition[1] === y
}

const getEnemyAt = (x: number, y: number): Enemy | undefined => {
  return props.enemies.find(e => e.position[0] === x && e.position[1] === y)
}

const isTrailAt = (x: number, y: number) => {
  return props.playerTrail.some(t => t[0] === x && t[1] === y)
}

const isVisible = (x: number, y: number) => {
  if (!props.fog || !props.fog[y]) return true
  return props.fog[y][x]
}

const getCellClass = (x: number, y: number) => {
  const classes: string[] = []

  if (!isVisible(x, y)) {
    classes.push('hidden')
    return classes
  }

  if (isPlayerAt(x, y)) {
    classes.push('player')
  } else if (getEnemyAt(x, y)) {
    classes.push('enemy')
    const enemy = getEnemyAt(x, y)
    if (enemy) {
      classes.push(`enemy-color-${enemy.colorIndex}`)
    }
  } else if (isTrailAt(x, y)) {
    classes.push('trail')
  } else if (props.mapData[y][x] === 1) {
    classes.push('wall')
  } else {
    classes.push('floor')
  }

  return classes
}

const getCellContent = (x: number, y: number) => {
  if (!isVisible(x, y)) return '?'
  if (isPlayerAt(x, y)) return '@'
  if (getEnemyAt(x, y)) return 'E'
  if (isTrailAt(x, y)) return '*'
  if (props.mapData[y][x] === 1) return '#'
  return '.'
}
</script>

<style scoped>
.game-map {
  background: #1a1a2e;
  padding: 10px;
  border-radius: 8px;
}

.map-grid {
  display: flex;
  flex-direction: column;
  gap: 0;
}

.map-row {
  display: flex;
  gap: 0;
}

.map-cell {
  width: 24px;
  height: 24px;
  display: flex;
  align-items: center;
  justify-content: center;
  font-family: 'Consolas', monospace;
  font-size: 14px;
  font-weight: bold;
  border-radius: 2px;
}

/* Cell types */
.hidden {
  background: #0f0f1a;
  color: #333;
}

.floor {
  background: #2d2d44;
  color: #666;
}

.wall {
  background: #4a4a6a;
  color: #8888aa;
}

.player {
  background: #2ecc71;
  color: #fff;
  animation: pulse 1s infinite;
}

.enemy {
  background: #e74c3c;
  color: #fff;
}

.enemy-color-1 { background: #e74c3c; }
.enemy-color-2 { background: #c0392b; }
.enemy-color-3 { background: #d35400; }
.enemy-color-4 { background: #e67e22; }

.trail {
  background: #27ae60;
  color: #2ecc71;
}

@keyframes pulse {
  0%, 100% { opacity: 1; }
  50% { opacity: 0.8; }
}
</style>
