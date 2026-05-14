<template>
  <div v-if="visible" class="combat-overlay">
    <div class="combat-modal">
      <div class="combat-header">
        <h3>⚔️ 战斗开始！</h3>
        <span class="enemy-name">vs {{ enemy?.id || '敌人' }}</span>
      </div>
      
      <div class="combat-body">
        <!-- 战斗双方状态 -->
        <div class="combatants">
          <div class="combatant player-side">
            <div class="avatar">🧙‍♂️</div>
            <div class="name">{{ player?.id || '玩家' }}</div>
            <div class="hp-bar">
              <div class="hp-fill" :style="{ width: playerHpPercent + '%' }"></div>
              <span class="hp-text">{{ player?.hp || 0 }}/{{ player?.maxHp || 20 }}</span>
            </div>
          </div>
          
          <div class="vs">VS</div>
          
          <div class="combatant enemy-side">
            <div class="avatar">👹</div>
            <div class="name">{{ enemy?.id || '敌人' }}</div>
            <div class="hp-bar">
              <div class="hp-fill enemy" :style="{ width: enemyHpPercent + '%' }"></div>
              <span class="hp-text">{{ enemy?.hp || 0 }}/{{ enemy?.maxHp || 10 }}</span>
            </div>
          </div>
        </div>
        
        <!-- 战斗日志 -->
        <div class="combat-log">
          <div class="log-title">战斗记录</div>
          <div class="log-content" ref="logContent">
            <div 
              v-for="(log, index) in combatLog" 
              :key="index"
              class="log-entry"
              :class="log.type"
            >
              <span class="round">{{ log.round }}</span>
              <span class="action">{{ log.message }}</span>
            </div>
          </div>
        </div>
      </div>
      
      <div class="combat-footer">
        <div v-if="result" class="result" :class="result">
          {{ result === 'win' ? '🎉 战斗胜利！' : '💀 战斗失败...' }}
        </div>
        <button 
          v-if="result" 
          class="btn-close-combat"
          @click="$emit('close')"
        >
          关闭
        </button>
      </div>
    </div>
  </div>
</template>

<script setup>
import { computed, ref, watch, nextTick } from 'vue'

const props = defineProps({
  visible: {
    type: Boolean,
    default: false
  },
  player: {
    type: Object,
    default: () => ({})
  },
  enemy: {
    type: Object,
    default: () => ({})
  },
  combatLog: {
    type: Array,
    default: () => []
  },
  result: {
    type: String,
    default: '' // 'win', 'lose', ''
  }
})

defineEmits(['close'])

const logContent = ref(null)

const playerHpPercent = computed(() => {
  const max = props.player?.maxHp || 20
  const hp = props.player?.hp || 0
  return Math.max(0, Math.min(100, (hp / max) * 100))
})

const enemyHpPercent = computed(() => {
  const max = props.enemy?.maxHp || 10
  const hp = props.enemy?.hp || 0
  return Math.max(0, Math.min(100, (hp / max) * 100))
})

// Auto-scroll log
watch(() => props.combatLog.length, async () => {
  await nextTick()
  if (logContent.value) {
    logContent.value.scrollTop = logContent.value.scrollHeight
  }
})
</script>

<style scoped>
.combat-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(92, 75, 60, 0.8);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 1000;
}

.combat-modal {
  background: #fcefd7;
  border: 3px solid #7da8b1;
  border-radius: 15px;
  width: 500px;
  max-width: 90vw;
  box-shadow: 0 10px 40px rgba(92, 75, 60, 0.4);
  overflow: hidden;
}

.combat-header {
  background: linear-gradient(135deg, #7da8b1 0%, #5a8a94 100%);
  color: #f5e2c3;
  padding: 15px 20px;
  text-align: center;
}

.combat-header h3 {
  margin: 0;
  font-size: 1.5rem;
}

.enemy-name {
  font-size: 0.9rem;
  opacity: 0.9;
}

.combat-body {
  padding: 20px;
}

.combatants {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 20px;
}

.combatant {
  text-align: center;
  flex: 1;
}

.avatar {
  font-size: 3rem;
  margin-bottom: 8px;
}

.name {
  font-weight: bold;
  color: #5a4a42;
  margin-bottom: 10px;
}

.hp-bar {
  position: relative;
  height: 20px;
  background: #e0d5c5;
  border-radius: 10px;
  overflow: hidden;
  border: 1px solid #b8a99a;
}

.hp-fill {
  height: 100%;
  background: linear-gradient(90deg, #7a9c58, #9c8d6d);
  transition: width 0.3s ease;
}

.hp-fill.enemy {
  background: linear-gradient(90deg, #c47e5a, #a65d3a);
}

.hp-text {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  font-size: 11px;
  font-weight: bold;
  color: #5a4a42;
}

.vs {
  font-size: 1.5rem;
  font-weight: bold;
  color: #c47e5a;
  padding: 0 15px;
}

.combat-log {
  background: #f5e2c3;
  border: 1px solid #d4c4a8;
  border-radius: 8px;
  padding: 10px;
}

.log-title {
  font-weight: bold;
  color: #5a4a42;
  margin-bottom: 8px;
  text-align: center;
  border-bottom: 1px dashed #d4c4a8;
  padding-bottom: 5px;
}

.log-content {
  height: 120px;
  overflow-y: auto;
  font-family: monospace;
  font-size: 12px;
}

.log-entry {
  padding: 3px 0;
  display: flex;
  gap: 8px;
}

.round {
  color: #7da8b1;
  font-weight: bold;
  min-width: 50px;
}

.action {
  color: #5a4a42;
}

.log-entry.damage .action {
  color: #c47e5a;
}

.log-entry.win .action {
  color: #7a9c58;
  font-weight: bold;
}

.log-entry.lose .action {
  color: #c47e5a;
  font-weight: bold;
}

.combat-footer {
  padding: 15px 20px;
  background: #f5e2c3;
  border-top: 1px solid #d4c4a8;
  text-align: center;
}

.result {
  font-size: 1.3rem;
  font-weight: bold;
  margin-bottom: 10px;
}

.result.win {
  color: #7a9c58;
}

.result.lose {
  color: #c47e5a;
}

.btn-close-combat {
  background: linear-gradient(135deg, #7da8b1 0%, #5a8a94 100%);
  color: #f5e2c3;
  border: none;
  padding: 10px 30px;
  border-radius: 8px;
  font-weight: bold;
  cursor: pointer;
  transition: all 0.2s ease;
}

.btn-close-combat:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 10px rgba(125, 168, 177, 0.4);
}
</style>
