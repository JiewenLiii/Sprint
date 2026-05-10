<template>
  <div v-if="visible" class="difficulty-overlay">
    <div class="difficulty-modal">
      <div class="modal-header">
        <h2>选择难度</h2>
        <p class="subtitle">选择适合你的挑战级别</p>
      </div>
      
      <div class="modal-body">
        <div class="difficulty-options">
          <div 
            class="difficulty-card"
            :class="{ selected: selected === 'easy' }"
            @click="selected = 'easy'"
          >
            <div class="difficulty-icon">🌱</div>
            <h3>简单</h3>
            <ul class="difficulty-stats">
              <li>敌人数量: 4</li>
              <li>敌人生命: 10</li>
              <li>敌人攻击: 3</li>
            </ul>
            <p class="difficulty-desc">适合新手玩家</p>
          </div>
          
          <div 
            class="difficulty-card"
            :class="{ selected: selected === 'hard' }"
            @click="selected = 'hard'"
          >
            <div class="difficulty-icon">🔥</div>
            <h3>困难</h3>
            <ul class="difficulty-stats">
              <li>敌人数量: 8</li>
              <li>敌人生命: 15</li>
              <li>敌人攻击: 6</li>
            </ul>
            <p class="difficulty-desc">适合资深玩家</p>
          </div>
        </div>
      </div>
      
      <div class="modal-footer">
        <button 
          class="btn-start"
          :disabled="!selected"
          @click="startGame"
        >
          开始游戏
        </button>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue'

const props = defineProps({
  visible: {
    type: Boolean,
    default: true
  }
})

const emit = defineEmits(['select'])

const selected = ref('')

const startGame = () => {
  if (selected.value) {
    emit('select', selected.value)
  }
}
</script>

<style scoped>
.difficulty-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(92, 75, 60, 0.85);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 2000;
}

.difficulty-modal {
  background: #fcefd7;
  border: 3px solid #7da8b1;
  border-radius: 20px;
  width: 600px;
  max-width: 90vw;
  box-shadow: 0 15px 50px rgba(92, 75, 60, 0.5);
  overflow: hidden;
}

.modal-header {
  background: linear-gradient(135deg, #7da8b1 0%, #5a8a94 100%);
  color: #f5e2c3;
  padding: 25px;
  text-align: center;
}

.modal-header h2 {
  margin: 0;
  font-size: 1.8rem;
}

.subtitle {
  margin: 8px 0 0 0;
  opacity: 0.9;
  font-size: 1rem;
}

.modal-body {
  padding: 30px;
}

.difficulty-options {
  display: flex;
  gap: 20px;
  justify-content: center;
}

.difficulty-card {
  flex: 1;
  background: #f5e2c3;
  border: 2px solid #d4c4a8;
  border-radius: 15px;
  padding: 20px;
  text-align: center;
  cursor: pointer;
  transition: all 0.3s ease;
}

.difficulty-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 8px 20px rgba(125, 168, 177, 0.3);
}

.difficulty-card.selected {
  border-color: #7da8b1;
  background: #e9edc9;
  box-shadow: 0 0 0 3px rgba(125, 168, 177, 0.3);
}

.difficulty-icon {
  font-size: 3rem;
  margin-bottom: 10px;
}

.difficulty-card h3 {
  color: #5a4a42;
  margin: 0 0 15px 0;
  font-size: 1.3rem;
}

.difficulty-stats {
  list-style: none;
  padding: 0;
  margin: 0 0 15px 0;
  text-align: left;
  font-size: 0.9rem;
  color: #5a4a42;
}

.difficulty-stats li {
  padding: 3px 0;
  border-bottom: 1px dashed rgba(125, 168, 177, 0.3);
}

.difficulty-stats li:last-child {
  border-bottom: none;
}

.difficulty-desc {
  color: #8a7a6b;
  font-size: 0.85rem;
  margin: 0;
}

.modal-footer {
  padding: 20px 30px 30px;
  text-align: center;
  background: #f5e2c3;
  border-top: 1px solid #d4c4a8;
}

.btn-start {
  background: linear-gradient(135deg, #7da8b1 0%, #5a8a94 100%);
  color: #f5e2c3;
  border: none;
  padding: 12px 40px;
  border-radius: 10px;
  font-size: 1.1rem;
  font-weight: bold;
  cursor: pointer;
  transition: all 0.2s ease;
}

.btn-start:hover:not(:disabled) {
  transform: translateY(-2px);
  box-shadow: 0 6px 15px rgba(125, 168, 177, 0.4);
}

.btn-start:disabled {
  background: #ccc;
  cursor: not-allowed;
  opacity: 0.6;
}
</style>
