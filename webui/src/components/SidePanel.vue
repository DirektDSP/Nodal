<script setup lang="ts">
defineProps<{
  currentView: 'main' | 'settings' | 'graph'
}>()

const emit = defineEmits<{
  navigate: [view: 'main' | 'settings' | 'graph']
}>()
</script>

<template>
  <aside class="side-panel">
    <nav class="nav-items">
      <button 
        class="nav-button"
        :class="{ active: currentView === 'graph' }"
        @click="emit('navigate', 'graph')"
        title="Graph Editor"
      >
        <svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <circle cx="6" cy="6" r="3" />
          <circle cx="18" cy="6" r="3" />
          <circle cx="18" cy="18" r="3" />
          <circle cx="6" cy="18" r="3" />
          <line x1="8.5" y1="7" x2="15.5" y2="7" />
          <line x1="8.5" y1="17" x2="15.5" y2="17" />
          <line x1="6" y1="9" x2="6" y2="15" />
          <line x1="18" y1="9" x2="18" y2="15" />
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

      <button 
        class="nav-button"
        :class="{ active: currentView === 'main' }"
        @click="emit('navigate', 'main')"
        title="Info"
      >
        <svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <circle cx="12" cy="12" r="10" />
          <line x1="12" y1="16" x2="12" y2="12" />
          <line x1="12" y1="8" x2="12.01" y2="8" />
        </svg>
      </button>
    </nav>
  </aside>
</template>

<style scoped>
.side-panel {
  width: 80px;
  background: var(--bg-secondary);
  border-right: 1px solid var(--border-color);
  display: flex;
  flex-direction: column;
  position: relative;
  z-index: 10;
}

.nav-items {
  display: flex;
  flex-direction: column;
  justify-content: space-evenly;
  align-items: center;
  height: 100%;
  padding: 2rem 0;
}

.nav-button {
  background: transparent;
  border: none;
  cursor: pointer;
  padding: 1rem;
  border-radius: 8px;
  transition: all 0.2s ease;
  display: flex;
  align-items: center;
  justify-content: center;
  color: var(--text-secondary);
}

.nav-button:hover {
  background: var(--hover-bg);
  color: var(--text-primary);
}

.nav-button.active {
  background: var(--accent-primary);
  color: var(--bg-primary);
}

.icon {
  width: 28px;
  height: 28px;
}
</style>
