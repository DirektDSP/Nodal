<script setup lang="ts">
import { ref, computed } from 'vue'
import { VueFlow, type Node, type Edge } from '@vue-flow/core'
import { Background } from '@vue-flow/background'
import { Controls } from '@vue-flow/controls'
import '@vue-flow/core/dist/style.css'
import '@vue-flow/core/dist/theme-default.css'

const emit = defineEmits<{
  close: []
}>()

// Node parameter storage
interface DelayParameters {
  delayTime: number
  feedback: number
  wetDry: number
  stereoWidth: number
  damping: number
  pingPongAmount: number
}

const nodeParameters = ref<Record<string, DelayParameters>>({
  '2': {
    delayTime: 250,
    feedback: 50,
    wetDry: 50,
    stereoWidth: 100,
    damping: 0,
    pingPongAmount: 75,
  },
  '3': {
    delayTime: 300,
    feedback: 60,
    wetDry: 50,
    stereoWidth: 100,
    damping: 10,
    pingPongAmount: 80,
  },
})

// Selected node tracking
const selectedNodeId = ref<string | null>('2')

// Current parameters (synced with selected node)
const currentParams = computed({
  get: () => {
    if (selectedNodeId.value && nodeParameters.value[selectedNodeId.value]) {
      return nodeParameters.value[selectedNodeId.value]
    }
    return null
  },
  set: (val) => {
    if (val && selectedNodeId.value) {
      nodeParameters.value[selectedNodeId.value] = val
    }
  }
})

const formatTime = (ms: number) => {
  if (ms < 1000) return `${ms}ms`
  return `${(ms / 1000).toFixed(2)}s`
}

// VueFlow graph setup
const nodes = ref<Node[]>([
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
    data: { type: 'delay' },
  },
  {
    id: '3',
    label: 'Delay R',
    position: { x: 250, y: 200 },
    data: { type: 'delay' },
  },
  {
    id: '4',
    type: 'output',
    label: 'Output',
    position: { x: 450, y: 150 },
  },
])

const edges = ref<Edge[]>([
  { id: 'e1-2', source: '1', target: '2', animated: true },
  { id: 'e1-3', source: '1', target: '3', animated: true },
  { id: 'e2-4', source: '2', target: '4', animated: true },
  { id: 'e3-4', source: '3', target: '4', animated: true },
])

// Context menu
const contextMenu = ref({
  show: false,
  x: 0,
  y: 0,
  nodeId: null as string | null,
})

let nextNodeId = 5

const onNodeClick = (event: { node: Node }) => {
  const nodeId = event.node.id
  if (nodeId !== '1' && nodeId !== '4' && nodes.value.find(n => n.id === nodeId)?.data?.type === 'delay') {
    selectedNodeId.value = nodeId
  }
}

const onPaneClick = () => {
  contextMenu.value.show = false
}

const onNodeContextMenu = (event: { node: Node; event: MouseEvent | TouchEvent }) => {
  event.event.preventDefault()
  const nodeId = event.node.id
  
  // Only show context menu for delay nodes
  if (nodeId !== '1' && nodeId !== '4' && nodes.value.find(n => n.id === nodeId)?.data?.type === 'delay') {
    const clientX = 'clientX' in event.event ? event.event.clientX : event.event.touches?.[0]?.clientX || 0
    const clientY = 'clientY' in event.event ? event.event.clientY : event.event.touches?.[0]?.clientY || 0
    
    contextMenu.value = {
      show: true,
      x: clientX,
      y: clientY,
      nodeId: nodeId,
    }
  }
}

const onPaneContextMenu = (event: MouseEvent | TouchEvent) => {
  event.preventDefault()
  const clientX = 'clientX' in event ? event.clientX : event.touches?.[0]?.clientX || 0
  const clientY = 'clientY' in event ? event.clientY : event.touches?.[0]?.clientY || 0
  
  contextMenu.value = {
    show: true,
    x: clientX,
    y: clientY,
    nodeId: null,
  }
}

const addNewDelay = () => {
  const newId = String(nextNodeId++)
  const newNode: Node = {
    id: newId,
    label: `Delay ${newId}`,
    position: { x: 250, y: 50 + nodes.value.length * 60 },
    data: { type: 'delay' },
  }
  
  nodes.value.push(newNode)
  
  // Initialize parameters for new node
  nodeParameters.value[newId] = {
    delayTime: 250,
    feedback: 50,
    wetDry: 50,
    stereoWidth: 100,
    damping: 0,
    pingPongAmount: 75,
  }
  
  contextMenu.value.show = false
}

const duplicateNode = () => {
  if (!contextMenu.value.nodeId) return
  
  const sourceNode = nodes.value.find(n => n.id === contextMenu.value.nodeId)
  if (!sourceNode) return
  
  const newId = String(nextNodeId++)
  const newNode: Node = {
    id: newId,
    label: `${sourceNode.label} Copy`,
    position: { 
      x: sourceNode.position.x + 50, 
      y: sourceNode.position.y + 50 
    },
    data: { type: 'delay' },
  }
  
  nodes.value.push(newNode)
  
  // Copy parameters from source node
  const sourceParams = nodeParameters.value[sourceNode.id]
  if (sourceParams) {
    nodeParameters.value[newId] = {
      delayTime: sourceParams.delayTime,
      feedback: sourceParams.feedback,
      wetDry: sourceParams.wetDry,
      stereoWidth: sourceParams.stereoWidth,
      damping: sourceParams.damping,
      pingPongAmount: sourceParams.pingPongAmount,
    }
  }
  
  contextMenu.value.show = false
}

const renameNode = () => {
  if (!contextMenu.value.nodeId) return
  
  const node = nodes.value.find(n => n.id === contextMenu.value.nodeId)
  if (!node) return
  
  const currentLabel = typeof node.label === 'string' ? node.label : 'Delay'
  const newName = prompt('Enter new name:', currentLabel)
  if (newName && newName.trim()) {
    node.label = newName.trim()
  }
  
  contextMenu.value.show = false
}

const deleteNode = () => {
  if (!contextMenu.value.nodeId) return
  
  const nodeId = contextMenu.value.nodeId
  
  // Remove node
  nodes.value = nodes.value.filter(n => n.id !== nodeId)
  
  // Remove connected edges
  edges.value = edges.value.filter(e => e.source !== nodeId && e.target !== nodeId)
  
  // Remove parameters
  delete nodeParameters.value[nodeId]
  
  // Clear selection if deleted node was selected
  if (selectedNodeId.value === nodeId) {
    selectedNodeId.value = null
  }
  
  contextMenu.value.show = false
}
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
          <p v-if="selectedNodeId">
            Editing: <strong>{{ nodes.find(n => n.id === selectedNodeId)?.label || 'Unknown' }}</strong>
          </p>
          <p v-else class="no-selection">Select a delay node to edit its parameters</p>
        </div>

        <div v-if="selectedNodeId" class="controls-grid">
          <!-- Delay Time -->
          <div class="control-group">
            <div class="control-header">
              <label>Delay Time</label>
              <span class="control-value">{{ formatTime(currentParams.delayTime) }}</span>
            </div>
            <input 
              type="range" 
              v-model.number="currentParams.delayTime" 
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
              <span class="control-value">{{ currentParams.feedback }}%</span>
            </div>
            <input 
              type="range" 
              v-model.number="currentParams.feedback" 
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
              <span class="control-value">{{ currentParams.wetDry }}%</span>
            </div>
            <input 
              type="range" 
              v-model.number="currentParams.wetDry" 
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
              <span class="control-value">{{ currentParams.pingPongAmount }}%</span>
            </div>
            <input 
              type="range" 
              v-model.number="currentParams.pingPongAmount" 
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
              <span class="control-value">{{ currentParams.stereoWidth }}%</span>
            </div>
            <input 
              type="range" 
              v-model.number="currentParams.stereoWidth" 
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
              <span class="control-value">{{ currentParams.damping }}%</span>
            </div>
            <input 
              type="range" 
              v-model.number="currentParams.damping" 
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
          @node-click="onNodeClick"
          @pane-click="onPaneClick"
          @node-context-menu="onNodeContextMenu"
          @pane-context-menu="onPaneContextMenu"
        >
          <Background pattern-color="#3b4261" :gap="16" />
          <Controls />
        </VueFlow>

        <!-- Context Menu -->
        <Teleport to="body">
          <div 
            v-if="contextMenu.show" 
            class="context-menu"
            :style="{ left: contextMenu.x + 'px', top: contextMenu.y + 'px' }"
            @click.stop
          >
            <template v-if="contextMenu.nodeId">
              <button @click="renameNode" class="context-menu-item">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <path d="M12 20h9M16.5 3.5a2.121 2.121 0 0 1 3 3L7 19l-4 1 1-4L16.5 3.5z" />
                </svg>
                Rename
              </button>
              <button @click="duplicateNode" class="context-menu-item">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <rect x="9" y="9" width="13" height="13" rx="2" ry="2" />
                  <path d="M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1" />
                </svg>
                Duplicate
              </button>
              <div class="context-menu-divider"></div>
              <button @click="deleteNode" class="context-menu-item danger">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <polyline points="3 6 5 6 21 6" />
                  <path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2" />
                </svg>
                Delete
              </button>
            </template>
            <template v-else>
              <button @click="addNewDelay" class="context-menu-item">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <line x1="12" y1="5" x2="12" y2="19" />
                  <line x1="5" y1="12" x2="19" y2="12" />
                </svg>
                Add Delay Node
              </button>
            </template>
          </div>
        </Teleport>
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

/* Context Menu */
.context-menu {
  position: fixed;
  background: var(--bg-secondary);
  border: 1px solid var(--border-color);
  border-radius: 8px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.4);
  z-index: 1000;
  min-width: 180px;
  padding: 0.5rem 0;
  animation: contextMenuFadeIn 0.15s ease-out;
}

@keyframes contextMenuFadeIn {
  from {
    opacity: 0;
    transform: scale(0.95);
  }
  to {
    opacity: 1;
    transform: scale(1);
  }
}

.context-menu-item {
  width: 100%;
  padding: 0.75rem 1rem;
  background: transparent;
  border: none;
  color: var(--text-primary);
  font-size: 0.9rem;
  text-align: left;
  cursor: pointer;
  transition: all 0.15s ease;
  display: flex;
  align-items: center;
  gap: 0.75rem;
}

.context-menu-item svg {
  width: 18px;
  height: 18px;
  flex-shrink: 0;
}

.context-menu-item:hover {
  background: var(--hover-bg);
}

.context-menu-item.danger {
  color: #ef4444;
}

.context-menu-item.danger:hover {
  background: rgba(239, 68, 68, 0.1);
}

.context-menu-divider {
  height: 1px;
  background: var(--border-color);
  margin: 0.5rem 0;
}

.no-selection {
  color: var(--text-secondary);
  font-style: italic;
}
</style>
