<script setup lang="ts">
import { ref } from 'vue'
import SidePanel from './SidePanel.vue'
import MainView from './MainView.vue'
import SettingsView from './SettingsView.vue'
import GraphEditorView from './GraphEditorView.vue'

type ViewState = 'main' | 'settings' | 'graph'

const currentView = ref<ViewState>('main')

const navigateTo = (view: ViewState) => {
  currentView.value = view
}
</script>

<template>
  <div class="plugin-container">
    <SidePanel 
      :currentView="currentView"
      @navigate="navigateTo"
    />
    
    <Transition name="view-transition" mode="out-in">
      <MainView v-if="currentView === 'main'" key="main" />
      <SettingsView v-else-if="currentView === 'settings'" key="settings" />
      <GraphEditorView v-else-if="currentView === 'graph'" key="graph" @close="navigateTo('main')" />
    </Transition>
  </div>
</template>

<style scoped>
.plugin-container {
  display: flex;
  width: 100%;
  height: 100vh;
  background: var(--bg-primary);
  overflow: hidden;
}

/* View transition animations */
.view-transition-enter-active,
.view-transition-leave-active {
  transition: all 0.3s ease;
}

.view-transition-enter-from {
  opacity: 0;
  transform: translateX(20px);
}

.view-transition-leave-to {
  opacity: 0;
  transform: translateX(-20px);
}
</style>
