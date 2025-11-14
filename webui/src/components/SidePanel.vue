<script setup lang="ts">
defineProps<{
  currentView: 'settings' | 'crt' | 'crt-graph'
}>()

const emit = defineEmits<{
  navigate: [view: 'settings' | 'crt' | 'crt-graph']
}>()
</script>

<template>
  <aside class="side-panel">
    <nav class="nav-items">
      <button 
        class="nav-button"
        :class="{ active: currentView === 'crt-graph' }"
        @click="emit('navigate', 'crt-graph')"
        title="CRT Graph Editor"
      >
        <svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <rect x="2" y="3" width="20" height="14" rx="2" />
          <circle cx="6" cy="10" r="2" />
          <circle cx="18" cy="10" r="2" />
          <line x1="8" y1="10" x2="16" y2="10" />
          <path d="M8 21h8" />
          <path d="M12 17v4" />
        </svg>
      </button>

      <button 
        class="nav-button"
        :class="{ active: currentView === 'crt' }"
        @click="emit('navigate', 'crt')"
        title="CRT Plugin UI"
      >
        <svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <rect x="2" y="3" width="20" height="14" rx="2" />
          <path d="M8 21h8" />
          <path d="M12 17v4" />
          <line x1="2" y1="7" x2="22" y2="7" />
          <circle cx="8" cy="10" r="1.5" />
          <circle cx="12" cy="10" r="1.5" />
          <circle cx="16" cy="10" r="1.5" />
        </svg>
      </button>

      <button 
        class="nav-button"
        :class="{ active: currentView === 'settings' }"
        @click="emit('navigate', 'settings')"
        title="Settings"
      >
        <svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <circle cx="12" cy="12" r="3" />
          <path d="M12 1v6m0 6v10M1 12h6m6 0h10" />
          <path d="M4 4l3.5 3.5M16.5 16.5L20 20M20 4l-3.5 3.5M7.5 16.5L4 20" />
        </svg>
      </button>
    </nav>
  </aside>
</template>

<style scoped>
.side-panel {
  width: 80px;
  background: #0a0a0a;
  border-right: 1px solid rgba(0, 255, 136, 0.2);
  display: flex;
  flex-direction: column;
  position: relative;
  z-index: 10;
  box-shadow: inset -5px 0 20px rgba(0, 0, 0, 0.8);
}

.nav-items {
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  height: 100%;
  padding: 2rem 0;
  gap: 2rem;
}

.nav-button {
  background: rgba(0, 0, 0, 0.5);
  border: 1px solid rgba(0, 255, 136, 0.2);
  cursor: pointer;
  padding: 1rem;
  transition: all 0.2s ease;
  display: flex;
  align-items: center;
  justify-content: center;
  color: #00ff88;
  clip-path: polygon(
    8px 0, calc(100% - 8px) 0, 100% 8px,
    100% calc(100% - 8px), calc(100% - 8px) 100%,
    8px 100%, 0 calc(100% - 8px), 0 8px
  );
}

.nav-button:hover {
  background: rgba(0, 255, 136, 0.1);
  border-color: #00ff88;
  box-shadow: 0 0 15px rgba(0, 255, 136, 0.3);
  transform: scale(1.05);
}

.nav-button.active {
  background: rgba(0, 255, 136, 0.2);
  border-color: #00ff88;
  box-shadow: 0 0 20px rgba(0, 255, 136, 0.5), inset 0 0 20px rgba(0, 255, 136, 0.2);
  color: #00ff88;
  text-shadow: 0 0 10px #00ff88;
}

.icon {
  width: 24px;
  height: 24px;
}
</style>
