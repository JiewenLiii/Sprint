<template>
  <div class="card control-card">
    <div class="card-header d-flex align-items-center">
      <i class="bi bi-keyboard me-2"></i>
      <h5 class="mb-0">操作说明</h5>
    </div>
    <div class="card-body">
      <div class="keyboard-guide">
        <div class="key-row">
          <div class="key-item">
            <span class="key">W</span>
            <span class="key-label">上</span>
          </div>
        </div>
        <div class="key-row">
          <div class="key-item">
            <span class="key">A</span>
            <span class="key-label">左</span>
          </div>
          <div class="key-item">
            <span class="key">S</span>
            <span class="key-label">下</span>
          </div>
          <div class="key-item">
            <span class="key">D</span>
            <span class="key-label">右</span>
          </div>
        </div>
        <div class="key-row mt-2">
          <div class="key-item">
            <span class="key">↑</span>
            <span class="key-label">上</span>
          </div>
          <div class="key-item">
            <span class="key">↓</span>
            <span class="key-label">下</span>
          </div>
          <div class="key-item">
            <span class="key">←</span>
            <span class="key-label">左</span>
          </div>
          <div class="key-item">
            <span class="key">→</span>
            <span class="key-label">右</span>
          </div>
        </div>
      </div>
      
      <hr class="my-3">
      
      <div class="action-buttons">
        <button 
          class="btn btn-restart w-100" 
          @click="$emit('restart')"
          :disabled="disabled"
        >
          <i class="bi bi-arrow-clockwise me-2"></i>
          重新开始
        </button>
      </div>
    </div>
  </div>
</template>

<script setup>
defineProps({
  disabled: {
    type: Boolean,
    default: false
  }
})

defineEmits(['restart'])
</script>

<style scoped>
.control-card {
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
}

.keyboard-guide {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 5px;
}

.key-row {
  display: flex;
  justify-content: center;
  gap: 8px;
}

.key-item {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 2px;
}

.key {
  width: 32px;
  height: 32px;
  background: linear-gradient(135deg, #e9edc9 0%, #faedcd 100%);
  border: 2px solid #7da8b1;
  border-radius: 6px;
  display: flex;
  align-items: center;
  justify-content: center;
  font-weight: bold;
  font-size: 14px;
  color: #5a4a42;
  box-shadow: 0 2px 4px rgba(125, 168, 177, 0.2);
}

.key-label {
  font-size: 10px;
  color: #8a7a6b;
}

hr {
  border-color: #7da8b1;
  opacity: 0.3;
}

.btn-restart {
  background: linear-gradient(135deg, #ccd5ae 0%, #e9edc9 100%);
  border: 2px solid #8b4513;
  color: #5d4037;
  font-weight: 600;
  padding: 10px;
  border-radius: 8px;
  transition: all 0.2s ease;
}

.btn-restart:hover:not(:disabled) {
  background: linear-gradient(135deg, #e9edc9 0%, #faedcd 100%);
  transform: translateY(-2px);
  box-shadow: 0 4px 10px rgba(139, 69, 19, 0.2);
}

.btn-restart:disabled {
  background: #e0e0e0;
  border-color: #ccc;
  color: #999;
  cursor: not-allowed;
}
</style>
