<script setup lang="ts">
import { ref } from 'vue'
import { VueFlow } from '@vue-flow/core'
import { Background } from '@vue-flow/background'
import { Controls } from '@vue-flow/controls'
import '@vue-flow/core/dist/style.css'
import '@vue-flow/core/dist/theme-default.css'

const emit = defineEmits<{
  close: []
}>()

// Delay parameters
const delayTime = ref(250)
const feedback = ref(50)
const wetDry = ref(50)
const stereoWidth = ref(100)
const damping = ref(0)
const pingPongAmount = ref(75)

const formatTime = (ms: number) => {
  if (ms < 1000) return `${ms}ms`
  return `${(ms / 1000).toFixed(2)}s`
}

// VueFlow graph setup
const nodes = ref([
  {
    id: '1',
    type: 'input',
    label: 'Input',
    position: { x: 50, y: 150 },
  },
  {
    id: '2',
    label: 'Delay L',
    position: { x: 250, y: 100 },
  },
  {
    id: '3',
    label: 'Delay R',
    position: { x: 250, y: 200 },
  },
  {
    id: '4',
    type: 'output',
    label: 'Output',
    position: { x: 450, y: 150 },
  },
])

const edges = ref([
  { id: 'e1-2', source: '1', target: '2', animated: true },
  { id: 'e1-3', source: '1', target: '3', animated: true },
  { id: 'e2-4', source: '2', target: '4', animated: true },
  { id: 'e3-4', source: '3', target: '4', animated: true },
])
</script>

<template>
  <div class="graph-editor-container">
    <!-- Main controls panel -->
    <main class="controls-panel">
      <button class="close-button" @click="emit('close')" title="Close Graph Editor">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <line x1="18" y1="6" x2="6" y2="18" />
          <line x1="6" y1="6" x2="18" y2="18" />
        </svg>
      </button>

      <div class="editor-content">
        <div class="editor-header">
          <h1>Ping Pong Delay</h1>
          <p>Configure your stereo delay effect</p>
        </div>

        <div class="controls-grid">
          <!-- Delay Time -->
          <div class="control-group">
            <div class="control-header">
              <label>Delay Time</label>
              <span class="control-value">{{ formatTime(delayTime) }}</span>
            </div>
            <input 
              type="range" 
              v-model="delayTime" 
              min="10" 
              max="2000" 
              step="1"
              class="slider"
            />
            <div class="control-marks">
              <span>10ms</span>
              <span>2s</span>
            </div>
          </div>

          <!-- Feedback -->
          <div class="control-group">
            <div class="control-header">
              <label>Feedback</label>
              <span class="control-value">{{ feedback }}%</span>
            </div>
            <input 
              type="range" 
              v-model="feedback" 
              min="0" 
              max="100" 
              step="1"
              class="slider"
            />
            <div class="control-marks">
              <span>0%</span>
              <span>100%</span>
            </div>
          </div>

          <!-- Wet/Dry Mix -->
          <div class="control-group">
            <div class="control-header">
              <label>Wet/Dry Mix</label>
              <span class="control-value">{{ wetDry }}%</span>
            </div>
            <input 
              type="range" 
              v-model="wetDry" 
              min="0" 
              max="100" 
              step="1"
              class="slider"
            />
            <div class="control-marks">
              <span>Dry</span>
              <span>Wet</span>
            </div>
          </div>

          <!-- Ping Pong Amount -->
          <div class="control-group">
            <div class="control-header">
              <label>Ping Pong Amount</label>
              <span class="control-value">{{ pingPongAmount }}%</span>
            </div>
            <input 
              type="range" 
              v-model="pingPongAmount" 
              min="0" 
              max="100" 
              step="1"
              class="slider"
            />
            <div class="control-marks">
              <span>Mono</span>
              <span>Full</span>
            </div>
          </div>

          <!-- Stereo Width -->
          <div class="control-group">
            <div class="control-header">
              <label>Stereo Width</label>
              <span class="control-value">{{ stereoWidth }}%</span>
            </div>
            <input 
              type="range" 
              v-model="stereoWidth" 
              min="0" 
              max="200" 
              step="1"
              class="slider"
            />
            <div class="control-marks">
              <span>Mono</span>
              <span>Wide</span>
            </div>
          </div>

          <!-- Damping -->
          <div class="control-group">
            <div class="control-header">
              <label>Damping</label>
              <span class="control-value">{{ damping }}%</span>
            </div>
            <input 
              type="range" 
              v-model="damping" 
              min="0" 
              max="100" 
              step="1"
              class="slider"
            />
            <div class="control-marks">
              <span>Bright</span>
              <span>Dark</span>
            </div>
          </div>
        </div>

        <div class="action-buttons">
          <button class="action-btn secondary">Reset</button>
          <button class="action-btn primary">Save Preset</button>
        </div>
      </div>
    </main>

    <!-- Graph visualization panel -->
    <aside class="graph-panel">
      <div class="graph-header">
        <h2>Signal Graph</h2>
      </div>
      <div class="graph-container">
        <VueFlow 
          :nodes="nodes" 
          :edges="edges"
          :default-zoom="1"
          :min-zoom="0.5"
          :max-zoom="2"
        >
          <Background pattern-color="#3b4261" :gap="16" />
          <Controls />
        </VueFlow>
      </div>
    </aside>
  </div>
</template>

<style scoped>
.graph-editor-container {
  display: flex;
  width: 100%;
  height: 100vh;
  background: var(--bg-primary);
}

.controls-panel {
  flex: 1;
  display: flex;
  flex-direction: column;
  background: var(--bg-primary);
  position: relative;
  overflow-y: auto;
  min-width: 0;
}

.graph-panel {
  width: 40%;
  max-width: 600px;
  min-width: 400px;
  background: var(--bg-secondary);
  border-left: 1px solid var(--border-color);
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.graph-header {
  padding: 2rem 2rem 1rem;
  border-bottom: 1px solid var(--border-color);
}

.graph-header h2 {
  font-size: 1.5rem;
  font-weight: 500;
  color: var(--text-primary);
}

.graph-container {
  flex: 1;
  position: relative;
  background: var(--bg-tertiary);
}

.close-button {
  position: absolute;
  top: 1.5rem;
  right: 1.5rem;
  width: 40px;
  height: 40px;
  background: var(--bg-secondary);
  border: 1px solid var(--border-color);
  border-radius: 8px;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.2s ease;
  z-index: 10;
  color: var(--text-secondary);
}

.close-button:hover {
  background: var(--hover-bg);
  border-color: var(--accent-primary);
  color: var(--accent-primary);
}

.close-button svg {
  width: 20px;
  height: 20px;
}

.editor-content {
  flex: 1;
  padding: 3rem;
  max-width: 1000px;
  margin: 0 auto;
  width: 100%;
}

.editor-header {
  margin-bottom: 3rem;
}

.editor-header h1 {
  font-size: 2.5rem;
  font-weight: 300;
  margin-bottom: 0.5rem;
  color: var(--text-primary);
}

.editor-header p {
  font-size: 1.125rem;
  color: var(--text-secondary);
}

.controls-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
  gap: 2.5rem;
  margin-bottom: 3rem;
}

.control-group {
  display: flex;
  flex-direction: column;
}

.control-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 1rem;
}

.control-header label {
  font-size: 1rem;
  font-weight: 500;
  color: var(--text-primary);
}

.control-value {
  font-size: 1rem;
  font-weight: 600;
  color: var(--accent-primary);
  font-variant-numeric: tabular-nums;
}

.slider {
  -webkit-appearance: none;
  appearance: none;
  width: 100%;
  height: 6px;
  border-radius: 3px;
  background: var(--bg-secondary);
  outline: none;
  margin: 0.5rem 0;
  cursor: pointer;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 20px;
  height: 20px;
  border-radius: 50%;
  background: var(--accent-primary);
  cursor: pointer;
  transition: all 0.2s ease;
}

.slider::-webkit-slider-thumb:hover {
  transform: scale(1.2);
  box-shadow: 0 0 0 4px rgba(99, 102, 241, 0.2);
}

.slider::-moz-range-thumb {
  width: 20px;
  height: 20px;
  border-radius: 50%;
  background: var(--accent-primary);
  cursor: pointer;
  border: none;
  transition: all 0.2s ease;
}

.slider::-moz-range-thumb:hover {
  transform: scale(1.2);
  box-shadow: 0 0 0 4px rgba(99, 102, 241, 0.2);
}

.control-marks {
  display: flex;
  justify-content: space-between;
  font-size: 0.75rem;
  color: var(--text-secondary);
  margin-top: 0.25rem;
}

.action-buttons {
  display: flex;
  gap: 1rem;
  justify-content: flex-end;
  padding-top: 2rem;
  border-top: 1px solid var(--border-color);
}

.action-btn {
  padding: 0.875rem 2rem;
  border-radius: 8px;
  font-size: 1rem;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.2s ease;
  border: none;
}

.action-btn.secondary {
  background: var(--bg-secondary);
  color: var(--text-primary);
  border: 1px solid var(--border-color);
}

.action-btn.secondary:hover {
  background: var(--hover-bg);
  border-color: var(--accent-primary);
}

.action-btn.primary {
  background: var(--accent-primary);
  color: white;
  border: 1px solid var(--accent-primary);
}

.action-btn.primary:hover {
  background: var(--accent-hover);
  transform: translateY(-1px);
  box-shadow: 0 4px 12px rgba(99, 102, 241, 0.3);
}

/* VueFlow custom styling */
:deep(.vue-flow__node) {
  background: var(--bg-secondary);
  border: 2px solid var(--accent-primary);
  border-radius: 8px;
  padding: 0.75rem 1.5rem;
  color: var(--text-primary);
  font-weight: 500;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.3);
}

:deep(.vue-flow__node.selected) {
  box-shadow: 0 0 0 3px rgba(99, 102, 241, 0.3);
}

:deep(.vue-flow__edge-path) {
  stroke: var(--accent-primary);
  stroke-width: 2;
}

:deep(.vue-flow__controls) {
  background: var(--bg-secondary);
  border: 1px solid var(--border-color);
  border-radius: 8px;
}

:deep(.vue-flow__controls-button) {
  background: transparent;
  border: none;
  border-bottom: 1px solid var(--border-color);
  color: var(--text-primary);
}

:deep(.vue-flow__controls-button:hover) {
  background: var(--hover-bg);
}

:deep(.vue-flow__controls-button:last-child) {
  border-bottom: none;
}
</style>
