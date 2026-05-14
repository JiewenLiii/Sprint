<template>
  <div class="card control-card">
    <div class="card-header d-flex align-items-center">
      <i class="bi bi-controller me-2"></i>
      <h5 class="mb-0">操作</h5>
    </div>
    <div class="card-body">
      <div class="hint-text">
        <kbd>W</kbd><kbd>A</kbd><kbd>S</kbd><kbd>D</kbd>
        <span class="mx-1">或</span>
        <kbd>↑</kbd><kbd>←</kbd><kbd>↓</kbd><kbd>→</kbd>
        <span class="d-block mt-1 text-muted">移动角色</span>
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
  padding: 8px 15px;
  flex-shrink: 0;
}

.card-body {
  background: #f5e2c3;
  border-radius: 0 0 7px 7px;
  padding: 12px;
  flex: 1;
  min-height: 0;
  display: flex;
  flex-direction: column;
  justify-content: center;
}

.hint-text {
  text-align: center;
  font-size: 0.85rem;
  color: #5a4a42;
}

.hint-text kbd {
  display: inline-block;
  padding: 2px 6px;
  font-size: 11px;
  background: linear-gradient(135deg, #e9edc9 0%, #faedcd 100%);
  border: 1px solid #7da8b1;
  border-radius: 4px;
  margin: 0 1px;
  box-shadow: 0 1px 2px rgba(125, 168, 177, 0.2);
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
