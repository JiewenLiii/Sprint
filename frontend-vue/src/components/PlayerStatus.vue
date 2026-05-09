<template>
  <div class="player-status">
    <h3>玩家状态</h3>
    <div class="status-item">
      <span class="label">ID:</span>
      <span class="value">{{ player.id }}</span>
    </div>
    <div class="status-item">
      <span class="label">HP:</span>
      <span class="value hp" :class="{ low: player.hp <= 5 }">{{ player.hp }}/{{ player.maxHp || 20 }}</span>
    </div>
    <div class="status-item">
      <span class="label">攻击力:</span>
      <span class="value">{{ player.attack }}</span>
    </div>
    <div class="status-item">
      <span class="label">位置:</span>
      <span class="value">({{ player.position[0] }}, {{ player.position[1] }})</span>
    </div>
    <div class="status-item">
      <span class="label">状态:</span>
      <span class="value" :class="player.isAlive ? 'alive' : 'dead'">
        {{ player.isAlive ? '✅ 存活' : '❌ 已死亡' }}
      </span>
    </div>
    <div class="status-item">
      <span class="label">敌人:</span>
      <span class="value">{{ enemyCount }}/{{ totalEnemies }}</span>
    </div>
  </div>
</template>

<script setup lang="ts">
interface Player {
  id: string
  position: number[]
  hp: number
  attack: number
  isAlive: boolean
  maxHp?: number
}

interface Props {
  player: Player
  enemyCount: number
  totalEnemies: number
}

defineProps<Props>()
</script>

<style scoped>
.player-status {
  background: #1a1a2e;
  padding: 15px;
  border-radius: 8px;
  color: #fff;
}

h3 {
  margin: 0 0 15px 0;
  color: #3498db;
  border-bottom: 1px solid #3498db;
  padding-bottom: 8px;
}

.status-item {
  display: flex;
  justify-content: space-between;
  padding: 8px 0;
  border-bottom: 1px solid #2d2d44;
}

.status-item:last-child {
  border-bottom: none;
}

.label {
  color: #888;
}

.value {
  color: #fff;
  font-weight: bold;
}

.value.hp {
  color: #2ecc71;
}

.value.hp.low {
  color: #e74c3c;
}

.value.alive {
  color: #2ecc71;
}

.value.dead {
  color: #e74c3c;
}
</style>
