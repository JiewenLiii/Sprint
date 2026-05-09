<template>
  <div class="card player-status-card h-100">
    <div class="card-header d-flex align-items-center">
      <i class="bi bi-person-circle me-2"></i>
      <h5 class="mb-0">玩家状态</h5>
    </div>
    <div class="card-body">
      <div class="player-info">
        <div class="info-item">
          <span class="info-label">ID:</span>
          <span class="info-value">{{ player.id || 'player_001' }}</span>
        </div>
        <div class="info-item">
          <span class="info-label">生命值:</span>
          <div class="hp-bar-container">
            <div class="hp-bar" :style="{ width: hpPercentage + '%' }" :class="hpClass"></div>
            <span class="hp-text">{{ player.hp }}/{{ player.maxHp || 20 }}</span>
          </div>
        </div>
        <div class="info-item">
          <span class="info-label">攻击力:</span>
          <span class="info-value attack">{{ player.attack || 5 }}</span>
        </div>
        <div class="info-item">
          <span class="info-label">位置:</span>
          <span class="info-value">({{ player.position?.[0] || 10 }}, {{ player.position?.[1] || 10 }})</span>
        </div>
        <div class="info-item">
          <span class="info-label">状态:</span>
          <span class="info-value" :class="player.isAlive !== false ? 'text-success' : 'text-danger'">
            {{ player.isAlive !== false ? '存活' : '已死亡' }}
          </span>
        </div>
        <div class="info-item">
          <span class="info-label">敌人:</span>
          <span class="info-value">{{ aliveEnemies }}/{{ totalEnemies }}</span>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { computed } from 'vue'

const props = defineProps({
  player: {
    type: Object,
    default: () => ({
      id: 'player_001',
      hp: 20,
      maxHp: 20,
      attack: 5,
      position: [10, 10],
      isAlive: true
    })
  },
  aliveEnemies: {
    type: Number,
    default: 0
  },
  totalEnemies: {
    type: Number,
    default: 4
  }
})

const hpPercentage = computed(() => {
  const maxHp = props.player.maxHp || 20
  const hp = props.player.hp || 0
  return Math.max(0, Math.min(100, (hp / maxHp) * 100))
})

const hpClass = computed(() => {
  const percentage = hpPercentage.value
  if (percentage <= 25) return 'hp-critical'
  if (percentage <= 50) return 'hp-warning'
  return 'hp-good'
})
</script>

<style scoped>
.player-status-card {
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
  padding: 8px 15px;
}

.card-body {
  background: #f5e2c3;
  border-radius: 0 0 7px 7px;
  padding: 12px;
  height: calc(100% - 35px);
  display: flex;
  flex-direction: column;
  overflow-y: auto;
}

.player-info {
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.info-item {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 6px 0;
  border-bottom: 1px dashed rgba(125, 168, 177, 0.3);
}

.info-item:last-child {
  border-bottom: none;
}

.info-label {
  font-weight: 600;
  color: #5a4a42;
}

.info-value {
  font-weight: bold;
  color: #5a4a42;
}

.info-value.attack {
  color: #c47e5a;
}

.hp-bar-container {
  position: relative;
  width: 100px;
  height: 18px;
  background: #fcefd7;
  border: 1px solid #7da8b1;
  border-radius: 10px;
  overflow: hidden;
}

.hp-bar {
  height: 100%;
  transition: width 0.3s ease;
  border-radius: 10px;
}

.hp-bar.hp-good {
  background: #9c8d6d;
}

.hp-bar.hp-warning {
  background: #c4a25a;
}

.hp-bar.hp-critical {
  background: #c47e5a;
}

.hp-text {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  font-size: 10px;
  font-weight: bold;
  color: #5a4a42;
}
</style>
