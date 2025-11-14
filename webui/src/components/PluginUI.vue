<script setup lang="ts">
import { ref } from 'vue'
import SidePanel from './SidePanel.vue'
import SettingsView from './SettingsView.vue'
import CRTPluginView from './CRTPluginView.vue'
import CRTGraphEditor from './CRTGraphEditor.vue'

type ViewState = 'settings' | 'crt' | 'crt-graph'

const currentView = ref<ViewState>('crt-graph')

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
      <CRTGraphEditor v-if="currentView === 'crt-graph'" key="crt-graph" />
      <CRTPluginView v-else-if="currentView === 'crt'" key="crt" />
      <SettingsView v-else-if="currentView === 'settings'" key="settings" />
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
