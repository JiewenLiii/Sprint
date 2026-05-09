<template>
  <div class="card log-card">
    <div class="card-header d-flex align-items-center">
      <i class="bi bi-journal-text me-2"></i>
      <h5 class="mb-0">游戏日志</h5>
    </div>
    <div class="card-body p-0">
      <div class="log-container" ref="logContainer">
        <div
          v-for="(log, index) in logs"
          :key="index"
          class="log-entry"
          :class="log.type"
        >
          <span class="log-time">[{{ log.time }}]</span>
          <span class="log-message">{{ log.message }}</span>
        </div>
        <div v-if="logs.length === 0" class="text-center text-muted py-3">
          <small>游戏日志将显示在这里...</small>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, watch, nextTick } from 'vue'

const props = defineProps({
  logs: {
    type: Array,
    default: () => []
  }
})

const logContainer = ref(null)

// Auto-scroll to bottom
watch(() => props.logs.length, async () => {
  await nextTick()
  if (logContainer.value) {
    logContainer.value.scrollTop = logContainer.value.scrollHeight
  }
})
</script>

<style scoped>
.log-card {
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

.log-container {
  height: 180px;
  overflow-y: auto;
  padding: 8px;
  background: #f5e2c3;
  border-radius: 0 0 7px 7px;
  font-family: monospace;
  font-size: 11px;
  white-space: pre-wrap;
}

.log-entry {
  padding: 3px 0;
  border-bottom: 1px dashed rgba(125, 168, 177, 0.2);
  display: flex;
  gap: 6px;
  align-items: flex-start;
}

.log-entry:last-child {
  border-bottom: none;
}

.log-time {
  color: #8a7a6b;
  flex-shrink: 0;
}

.log-message {
  color: #5a4a42;
  word-break: break-word;
}

.log-entry.success .log-message {
  color: #7a9c58;
}

.log-entry.error .log-message {
  color: #c47e5a;
}

.log-entry.warning .log-message {
  color: #c4a25a;
}

.log-entry.combat .log-message {
  color: #9c8d6d;
}
</style>
