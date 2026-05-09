<template>
  <div class="game-log">
    <h3>日志</h3>
    <div class="log-container" ref="logContainer">
      <div
        v-for="(log, index) in logs"
        :key="index"
        class="log-entry"
        :class="log.type"
      >
        <span class="timestamp">[{{ log.time }}]</span>
        <span class="icon">{{ getIcon(log.type) }}</span>
        <span class="message">{{ log.message }}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, watch, nextTick } from 'vue'

interface LogEntry {
  time: string
  message: string
  type: 'info' | 'success' | 'error' | 'warning'
}

interface Props {
  logs: LogEntry[]
}

const props = defineProps<Props>()
const logContainer = ref<HTMLElement | null>(null)

const getIcon = (type: string) => {
  switch (type) {
    case 'success': return '✅'
    case 'error': return '❌'
    case 'warning': return '⚠️'
    default: return 'ℹ️'
  }
}

// Auto-scroll to bottom when new logs arrive
watch(() => props.logs.length, async () => {
  await nextTick()
  if (logContainer.value) {
    logContainer.value.scrollTop = logContainer.value.scrollHeight
  }
})
</script>

<style scoped>
.game-log {
  background: #1a1a2e;
  padding: 15px;
  border-radius: 8px;
  color: #fff;
  height: 100%;
  display: flex;
  flex-direction: column;
}

h3 {
  margin: 0 0 10px 0;
  color: #f39c12;
}

.log-container {
  flex: 1;
  overflow-y: auto;
  background: #0f0f1a;
  border-radius: 4px;
  padding: 10px;
  font-family: 'Consolas', monospace;
  font-size: 12px;
  max-height: 300px;
}

.log-entry {
  padding: 4px 0;
  border-bottom: 1px solid #1a1a2e;
  display: flex;
  gap: 5px;
}

.log-entry:last-child {
  border-bottom: none;
}

.timestamp {
  color: #666;
}

.icon {
  flex-shrink: 0;
}

.message {
  color: #fff;
}

.log-entry.success .message { color: #2ecc71; }
.log-entry.error .message { color: #e74c3c; }
.log-entry.warning .message { color: #f39c12; }
.log-entry.info .message { color: #3498db; }
</style>
