<template>
  <div class="control-panel">
    <h3>方向键</h3>
    <div class="directional-controls">
      <button class="btn-direction" @click="$emit('move', 'up')" :disabled="disabled">
        ↑
      </button>
      <div class="btn-row">
        <button class="btn-direction" @click="$emit('move', 'left')" :disabled="disabled">
          ←
        </button>
        <button class="btn-direction" @click="$emit('move', 'down')" :disabled="disabled">
          ↓
        </button>
        <button class="btn-direction" @click="$emit('move', 'right')" :disabled="disabled">
          →
        </button>
      </div>
    </div>
    <div class="action-buttons">
      <button class="btn-action" @click="$emit('restart')" :disabled="disabled">
        重新开始
      </button>
    </div>
  </div>
</template>

<script setup lang="ts">
interface Props {
  disabled?: boolean
}

defineProps<Props>()
defineEmits(['move', 'restart'])
</script>

<style scoped>
.control-panel {
  background: #1a1a2e;
  padding: 15px;
  border-radius: 8px;
  color: #fff;
}

h3 {
  margin: 0 0 15px 0;
  color: #9b59b6;
  text-align: center;
}

.directional-controls {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 5px;
  margin-bottom: 15px;
}

.btn-row {
  display: flex;
  gap: 5px;
}

.btn-direction {
  width: 50px;
  height: 50px;
  font-size: 20px;
  background: #3498db;
  border: none;
  border-radius: 8px;
  color: white;
  cursor: pointer;
  transition: all 0.2s;
}

.btn-direction:hover:not(:disabled) {
  background: #2980b9;
  transform: scale(1.05);
}

.btn-direction:disabled {
  background: #555;
  cursor: not-allowed;
  opacity: 0.5;
}

.action-buttons {
  display: flex;
  justify-content: center;
}

.btn-action {
  padding: 10px 20px;
  background: #e67e22;
  border: none;
  border-radius: 8px;
  color: white;
  cursor: pointer;
  font-size: 14px;
  transition: all 0.2s;
}

.btn-action:hover:not(:disabled) {
  background: #d35400;
}

.btn-action:disabled {
  background: #555;
  cursor: not-allowed;
  opacity: 0.5;
}
</style>
