<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import { VueFlow, type Node, type Edge, type Connection, ConnectionMode, useVueFlow } from '@vue-flow/core'
import { Background } from '@vue-flow/background'
import { Controls } from '@vue-flow/controls'
import '@vue-flow/core/dist/style.css'
import '@vue-flow/core/dist/theme-default.css'
import DigitalSlider from './controls/DigitalSlider.vue'
import DigitalDial from './controls/DigitalDial.vue'
import DigitalToggle from './controls/DigitalToggle.vue'

type ColorTheme = 'green' | 'orange' | 'magenta'

const currentTheme = ref<ColorTheme>('green')
const scanlineEnabled = ref(true)

const themeColors: Record<ColorTheme, string> = {
  green: '#00ff88',
  orange: '#ff9500',
  magenta: '#ff006e'
}

const setTheme = (theme: ColorTheme) => {
  currentTheme.value = theme
  const color = themeColors[theme]
  
  // Parse hex to RGB for rgba usage
  const r = parseInt(color.slice(1, 3), 16)
  const g = parseInt(color.slice(3, 5), 16)
  const b = parseInt(color.slice(5, 7), 16)
  
  document.documentElement.style.setProperty('--crt-primary', color)
  document.documentElement.style.setProperty('--crt-primary-rgb', `${r}, ${g}, ${b}`)
}

// Initialize theme
setTheme(currentTheme.value)

// VueFlow instance
const { fitView, setCenter, zoomTo } = useVueFlow()

// Auto-fit on mount
onMounted(() => {
  setTimeout(() => {
    fitView({ padding: 0.2, duration: 300 })
  }, 100)
})

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

// VueFlow graph setup
const nodes = ref<Node[]>([
  {
    id: '1',
    type: 'input',
    label: 'Input',
    position: { x: 250, y: 50 },
  },
  {
    id: '2',
    label: 'Delay',
    position: { x: 350, y: 200 },
    data: { type: 'delay' },
  },
  {
    id: '4',
    type: 'output',
    label: 'Output',
    position: { x: 250, y: 350 },
  },
])

const edges = ref<Edge[]>([
  { id: 'e1-2', source: '1', target: '2', animated: true },
  { id: 'e1-4', source: '1', target: '4', animated: true },
  { id: 'e2-4', source: '2', target: '4', animated: true },
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
  
  // Find a good Y position between Input and Output
  const delayNodes = nodes.value.filter(n => n.data?.type === 'delay')
  const yPosition = 150 + (delayNodes.length * 80)
  const xPosition = 150 + (delayNodes.length % 3) * 150
  
  const newNode: Node = {
    id: newId,
    label: `Delay ${newId}`,
    position: { x: xPosition, y: yPosition },
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

const onConnect = (params: Connection) => {
  const newEdge: Edge = {
    id: `e${params.source}-${params.target}-${Date.now()}`,
    source: params.source,
    target: params.target,
    sourceHandle: params.sourceHandle || undefined,
    targetHandle: params.targetHandle || undefined,
    animated: true,
  }
  edges.value.push(newEdge)
}

const fitGraph = () => {
  fitView({ padding: 0.2, duration: 300 })
}

const centerGraph = () => {
  const allNodes = nodes.value
  if (allNodes.length === 0) return
  
  // Calculate center of all nodes
  const sumX = allNodes.reduce((sum, node) => sum + node.position.x, 0)
  const sumY = allNodes.reduce((sum, node) => sum + node.position.y, 0)
  const centerX = sumX / allNodes.length
  const centerY = sumY / allNodes.length
  
  setCenter(centerX, centerY, { duration: 300, zoom: 1 })
}

const resetZoom = () => {
  zoomTo(1, { duration: 300 })
}
</script>

<template>
  <div class="crt-graph-editor" :class="{ 'scanline-active': scanlineEnabled, [`theme-${currentTheme}`]: true }">
    <div class="scanline-overlay"></div>
    <div class="crt-screen">
      <!-- Header Bar -->
      <div class="crt-header">
        <div class="header-left">
          <div class="terminal-prompt">
            <span class="prompt-symbol">&gt;</span>
            <span class="prompt-text">PING_PONG_DELAY_EDITOR</span>
            <span class="cursor-blink">_</span>
          </div>
        </div>
        <div class="header-right">
          <div class="system-status">
            <span class="status-label">NODE:</span>
            <span class="status-value active">
              {{ selectedNodeId ? nodes.find(n => n.id === selectedNodeId)?.label || 'NONE' : 'NONE' }}
            </span>
          </div>
          <div class="theme-selector">
            <button 
              v-for="theme in ['green', 'orange', 'magenta'] as ColorTheme[]" 
              :key="theme"
              class="theme-btn"
              :class="{ active: currentTheme === theme }"
              @click="setTheme(theme)"
              :style="{ '--theme-color': themeColors[theme] }"
            >
              <div class="theme-dot"></div>
            </button>
          </div>
          <DigitalToggle 
            v-model="scanlineEnabled" 
            label="SCAN"
            size="small"
            :colorOn="themeColors[currentTheme]"
          />
        </div>
      </div>

      <!-- Main Content Area -->
      <div class="crt-content">
        <!-- Left Panel - Controls -->
        <div class="controls-panel">
          <div class="panel-header">
            <div class="panel-title">// DELAY_PARAMETERS</div>
            <div class="panel-line"></div>
          </div>
          <div class="panel-body">
            <div v-if="selectedNodeId && currentParams" class="controls-grid">
              <DigitalDial 
                v-model="currentParams.delayTime" 
                label="DELAY_MS"
                :min="10"
                :max="2000"
                :step="1"
                :size="110"
                :segments="20"
                :color="themeColors[currentTheme]"
              />
              <DigitalDial 
                v-model="currentParams.feedback" 
                label="FEEDBACK"
                :min="0"
                :max="100"
                :size="110"
                :segments="20"
                :color="themeColors[currentTheme]"
              />
              <DigitalDial 
                v-model="currentParams.wetDry" 
                label="WET_DRY"
                :min="0"
                :max="100"
                :size="110"
                :segments="20"
                :color="themeColors[currentTheme]"
              />
              <DigitalDial 
                v-model="currentParams.pingPongAmount" 
                label="PING_PONG"
                :min="0"
                :max="100"
                :size="110"
                :segments="20"
                :color="themeColors[currentTheme]"
              />
              <DigitalSlider 
                v-model="currentParams.stereoWidth" 
                label="STEREO_WIDTH"
                :min="0"
                :max="200"
                :color="themeColors[currentTheme]"
              />
              <DigitalSlider 
                v-model="currentParams.damping" 
                label="DAMPING"
                :min="0"
                :max="100"
                :color="themeColors[currentTheme]"
              />
            </div>
            <div v-else class="no-selection">
              <div class="no-selection-icon">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                  <circle cx="12" cy="12" r="3" />
                  <path d="M12 1v6m0 6v10" />
                  <circle cx="12" cy="12" r="10" />
                </svg>
              </div>
              <p>// SELECT_A_DELAY_NODE</p>
              <p class="hint">RIGHT_CLICK_CANVAS_TO_ADD</p>
            </div>
          </div>
        </div>

        <!-- Right Panel - Graph -->
        <div class="graph-panel">
          <div class="panel-header">
            <div class="panel-title-row">
              <div class="panel-title">// SIGNAL_GRAPH</div>
              <div class="graph-controls">
                <button @click="fitGraph" class="graph-btn" title="Fit to View">
                  <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <path d="M8 3H5a2 2 0 0 0-2 2v3m18 0V5a2 2 0 0 0-2-2h-3m0 18h3a2 2 0 0 0 2-2v-3M3 16v3a2 2 0 0 0 2 2h3" />
                  </svg>
                </button>
                <button @click="centerGraph" class="graph-btn" title="Center">
                  <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <circle cx="12" cy="12" r="3" />
                    <circle cx="12" cy="12" r="10" />
                  </svg>
                </button>
                <button @click="resetZoom" class="graph-btn" title="Reset Zoom">
                  <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <circle cx="11" cy="11" r="8" />
                    <path d="m21 21-4.35-4.35" />
                  </svg>
                </button>
              </div>
            </div>
            <div class="panel-line"></div>
          </div>
          <div class="graph-container">
            <VueFlow 
              :nodes="nodes" 
              :edges="edges"
              :default-zoom="1"
              :min-zoom="0.5"
              :max-zoom="2"
              :connection-mode="ConnectionMode.Loose"
              @node-click="onNodeClick"
              @pane-click="onPaneClick"
              @node-context-menu="onNodeContextMenu"
              @pane-context-menu="onPaneContextMenu"
              @connect="onConnect"
            >
              <Background pattern-color="var(--crt-primary)" :gap="16" :size="0.5" />
              <Controls />
            </VueFlow>
          </div>
        </div>
      </div>

      <!-- Status Bar -->
      <div class="crt-footer">
        <div class="footer-section">
          <span class="footer-label">NODES:</span>
          <span class="footer-value">{{ nodes.length }}</span>
        </div>
        <div class="footer-section">
          <span class="footer-label">EDGES:</span>
          <span class="footer-value">{{ edges.length }}</span>
        </div>
        <div class="footer-section">
          <span class="footer-label">SELECTED:</span>
          <span class="footer-value">{{ selectedNodeId || 'NONE' }}</span>
        </div>
        <div class="footer-section">
          <span class="footer-label">THEME:</span>
          <span class="footer-value">{{ currentTheme.toUpperCase() }}</span>
        </div>
      </div>
    </div>

    <!-- Context Menu -->
    <Teleport to="body">
      <div 
        v-if="contextMenu.show" 
        class="crt-context-menu"
        :style="{ left: contextMenu.x + 'px', top: contextMenu.y + 'px' }"
        @click.stop
      >
        <template v-if="contextMenu.nodeId">
          <button @click="renameNode" class="context-item">
            <span class="context-icon">►</span>
            <span>RENAME_NODE</span>
          </button>
          <button @click="duplicateNode" class="context-item">
            <span class="context-icon">►</span>
            <span>DUPLICATE_NODE</span>
          </button>
          <div class="context-divider"></div>
          <button @click="deleteNode" class="context-item danger">
            <span class="context-icon">✕</span>
            <span>DELETE_NODE</span>
          </button>
        </template>
        <template v-else>
          <button @click="addNewDelay" class="context-item">
            <span class="context-icon">+</span>
            <span>ADD_DELAY_NODE</span>
          </button>
        </template>
      </div>
    </Teleport>

    <!-- CRT Effects -->
    <div class="crt-curvature"></div>
    <div class="crt-vignette"></div>
  </div>
</template>

<style scoped>
.crt-graph-editor {
  width: 100%;
  height: 100vh;
  background: #0a0a0a;
  position: relative;
  overflow: hidden;
  font-family: 'Courier New', monospace;
  --crt-primary: #00ff88;
}

.theme-green {
  --crt-primary: #00ff88;
}

.theme-orange {
  --crt-primary: #ff9500;
}

.theme-magenta {
  --crt-primary: #ff006e;
}

.crt-screen {
  position: relative;
  width: 100%;
  height: 100%;
  background: radial-gradient(ellipse at center, #111 0%, #000 100%);
  padding: 1.5rem;
  display: flex;
  flex-direction: column;
  gap: 1rem;
  z-index: 1;
}

/* Scanline Effect */
.scanline-overlay {
  position: fixed;
  inset: 0;
  background: repeating-linear-gradient(
    0deg,
    rgba(0, 0, 0, 0.15) 0px,
    rgba(0, 0, 0, 0.15) 1px,
    transparent 1px,
    transparent 2px
  );
  pointer-events: none;
  z-index: 100;
  opacity: 0;
  transition: opacity 0.3s ease;
}

.scanline-active .scanline-overlay {
  opacity: 1;
  animation: scanlineScroll 8s linear infinite;
}

@keyframes scanlineScroll {
  0% {
    transform: translateY(0);
  }
  100% {
    transform: translateY(2px);
  }
}

/* CRT Effects */
.crt-curvature {
  position: fixed;
  inset: 0;
  background: radial-gradient(ellipse at center, transparent 0%, rgba(0, 0, 0, 0.3) 100%);
  pointer-events: none;
  z-index: 99;
}

.crt-vignette {
  position: fixed;
  inset: 0;
  box-shadow: inset 0 0 100px rgba(0, 0, 0, 0.8), inset 0 0 200px rgba(0, 0, 0, 0.6);
  pointer-events: none;
  z-index: 98;
}

/* Header */
.crt-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 1rem 1.5rem;
  background: rgba(0, 0, 0, 0.6);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.3);
  clip-path: polygon(
    20px 0, calc(100% - 20px) 0, 100% 20px,
    100% 100%, 0 100%, 0 20px
  );
  box-shadow: 
    0 0 20px rgba(var(--crt-primary-rgb, 0, 255, 136), 0.2),
    inset 0 0 30px rgba(0, 0, 0, 0.5);
}

.terminal-prompt {
  font-size: 1.25rem;
  font-weight: 700;
  color: var(--crt-primary);
  text-shadow: 0 0 10px var(--crt-primary);
  display: flex;
  gap: 0.5rem;
  align-items: center;
}

.prompt-symbol {
  font-size: 1.5rem;
}

.cursor-blink {
  animation: blink 1s step-end infinite;
}

.header-right {
  display: flex;
  gap: 2rem;
  align-items: center;
}

.system-status {
  display: flex;
  gap: 0.5rem;
  font-size: 0.875rem;
}

.status-label {
  color: var(--text-secondary);
}

.status-value {
  color: var(--crt-primary);
  font-weight: 700;
  text-shadow: 0 0 10px var(--crt-primary);
}

.theme-selector {
  display: flex;
  gap: 0.5rem;
}

.theme-btn {
  width: 20px;
  height: 20px;
  background: rgba(0, 0, 0, 0.5);
  border: 1px solid #333;
  padding: 3px;
  cursor: pointer;
  transition: all 0.2s ease;
  clip-path: polygon(4px 0, calc(100% - 4px) 0, 100% 4px, 100% calc(100% - 4px), calc(100% - 4px) 100%, 4px 100%, 0 calc(100% - 4px), 0 4px);
}

.theme-btn:hover {
  border-color: var(--theme-color);
  transform: scale(1.1);
}

.theme-btn.active {
  border-color: var(--theme-color);
  box-shadow: 0 0 10px var(--theme-color);
}

.theme-dot {
  width: 100%;
  height: 100%;
  background: var(--theme-color);
  clip-path: polygon(2px 0, calc(100% - 2px) 0, 100% 2px, 100% calc(100% - 2px), calc(100% - 2px) 100%, 2px 100%, 0 calc(100% - 2px), 0 2px);
  box-shadow: inset 0 0 5px var(--theme-color);
}

/* Content */
.crt-content {
  flex: 1;
  display: grid;
  grid-template-columns: 45% 55%;
  gap: 1.5rem;
  overflow: hidden;
}

.controls-panel,
.graph-panel {
  background: rgba(0, 0, 0, 0.6);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.2);
  clip-path: polygon(
    15px 0, calc(100% - 15px) 0, 100% 15px,
    100% calc(100% - 15px), calc(100% - 15px) 100%,
    15px 100%, 0 calc(100% - 15px), 0 15px
  );
  display: flex;
  flex-direction: column;
  box-shadow: inset 0 0 30px rgba(0, 0, 0, 0.5);
  overflow: hidden;
}

.panel-header {
  padding: 1rem 1.5rem;
  border-bottom: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.2);
}

.panel-title-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 0.5rem;
}

.panel-title {
  font-size: 0.875rem;
  font-weight: 700;
  color: var(--crt-primary);
  text-shadow: 0 0 10px var(--crt-primary);
  letter-spacing: 0.1em;
}

.panel-line {
  height: 2px;
  background: linear-gradient(90deg, var(--crt-primary) 0%, transparent 50%);
  margin-top: 0.5rem;
  box-shadow: 0 0 5px var(--crt-primary);
}

.panel-body {
  flex: 1;
  padding: 1.5rem;
  overflow-y: auto;
  display: flex;
  flex-direction: column;
}

.controls-grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 2rem;
  align-items: start;
  justify-items: center;
}

.controls-grid > :nth-last-child(1),
.controls-grid > :nth-last-child(2) {
  grid-column: 1 / -1;
  width: 100%;
  max-width: 500px;
  justify-self: center;
}

.no-selection {
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 1rem;
  color: var(--text-secondary);
  text-align: center;
}

.no-selection-icon {
  width: 80px;
  height: 80px;
  color: var(--crt-primary);
  opacity: 0.3;
  animation: pulse 2s ease-in-out infinite;
}

.no-selection-icon svg {
  width: 100%;
  height: 100%;
}

.no-selection p {
  font-size: 0.875rem;
  font-weight: 700;
  letter-spacing: 0.1em;
  margin: 0;
}

.no-selection .hint {
  font-size: 0.75rem;
  opacity: 0.6;
}

.graph-controls {
  display: flex;
  gap: 0.5rem;
}

.graph-btn {
  width: 28px;
  height: 28px;
  background: rgba(0, 0, 0, 0.5);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.3);
  color: var(--crt-primary);
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.2s ease;
  clip-path: polygon(4px 0, calc(100% - 4px) 0, 100% 4px, 100% calc(100% - 4px), calc(100% - 4px) 100%, 4px 100%, 0 calc(100% - 4px), 0 4px);
}

.graph-btn svg {
  width: 14px;
  height: 14px;
}

.graph-btn:hover {
  background: rgba(var(--crt-primary-rgb, 0, 255, 136), 0.1);
  border-color: var(--crt-primary);
  box-shadow: 0 0 10px var(--crt-primary);
  transform: scale(1.05);
}

.graph-container {
  flex: 1;
  position: relative;
  background: rgba(0, 0, 0, 0.3);
  overflow: hidden;
}

/* Style VueFlow to match CRT theme */
.graph-container :deep(.vue-flow) {
  background: transparent;
}

.graph-container :deep(.vue-flow__node) {
  background: rgba(0, 0, 0, 0.8);
  border: 1px solid var(--crt-primary);
  border-radius: 0;
  clip-path: polygon(8px 0, calc(100% - 8px) 0, 100% 8px, 100% calc(100% - 8px), calc(100% - 8px) 100%, 8px 100%, 0 calc(100% - 8px), 0 8px);
  color: var(--crt-primary);
  font-family: 'Courier New', monospace;
  font-weight: 700;
  box-shadow: 0 0 15px rgba(var(--crt-primary-rgb, 0, 255, 136), 0.5);
}

.graph-container :deep(.vue-flow__node.selected) {
  box-shadow: 0 0 25px var(--crt-primary);
  border-color: var(--crt-primary);
}

.graph-container :deep(.vue-flow__edge-path) {
  stroke: var(--crt-primary);
  stroke-width: 2;
}

.graph-container :deep(.vue-flow__controls) {
  background: rgba(0, 0, 0, 0.8);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.3);
  border-radius: 0;
  clip-path: polygon(6px 0, calc(100% - 6px) 0, 100% 6px, 100% calc(100% - 6px), calc(100% - 6px) 100%, 6px 100%, 0 calc(100% - 6px), 0 6px);
}

.graph-container :deep(.vue-flow__controls-button) {
  background: rgba(0, 0, 0, 0.5);
  border-bottom: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.2);
  color: var(--crt-primary);
}

.graph-container :deep(.vue-flow__controls-button:hover) {
  background: rgba(var(--crt-primary-rgb, 0, 255, 136), 0.2);
}

/* Footer */
.crt-footer {
  display: flex;
  gap: 2rem;
  padding: 0.75rem 1.5rem;
  background: rgba(0, 0, 0, 0.8);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.2);
  clip-path: polygon(
    0 0, 100% 0, 100% calc(100% - 15px),
    calc(100% - 15px) 100%, 15px 100%, 0 calc(100% - 15px)
  );
  box-shadow: 0 0 20px rgba(0, 0, 0, 0.5);
}

.footer-section {
  display: flex;
  gap: 0.75rem;
  align-items: center;
  font-size: 0.75rem;
}

.footer-label {
  color: var(--text-secondary);
  font-weight: 700;
}

.footer-value {
  color: var(--crt-primary);
  font-weight: 700;
  font-variant-numeric: tabular-nums;
  text-shadow: 0 0 5px var(--crt-primary);
}

/* Context Menu */
.crt-context-menu {
  position: fixed;
  background: rgba(0, 0, 0, 0.95);
  border: 1px solid var(--crt-primary);
  clip-path: polygon(
    10px 0, calc(100% - 10px) 0, 100% 10px,
    100% calc(100% - 10px), calc(100% - 10px) 100%,
    10px 100%, 0 calc(100% - 10px), 0 10px
  );
  box-shadow: 0 0 30px rgba(var(--crt-primary-rgb, 0, 255, 136), 0.5);
  z-index: 1000;
  min-width: 200px;
  padding: 0.5rem 0;
}

.context-item {
  width: 100%;
  padding: 0.75rem 1rem;
  background: transparent;
  border: none;
  color: var(--crt-primary);
  font-family: 'Courier New', monospace;
  font-size: 0.875rem;
  font-weight: 700;
  text-align: left;
  cursor: pointer;
  display: flex;
  align-items: center;
  gap: 0.75rem;
  transition: all 0.2s ease;
}

.context-item:hover {
  background: rgba(var(--crt-primary-rgb, 0, 255, 136), 0.2);
  text-shadow: 0 0 10px var(--crt-primary);
}

.context-item.danger {
  color: #ff006e;
}

.context-item.danger:hover {
  background: rgba(255, 0, 110, 0.2);
  text-shadow: 0 0 10px #ff006e;
}

.context-icon {
  font-size: 1rem;
  opacity: 0.7;
}

.context-divider {
  height: 1px;
  background: rgba(var(--crt-primary-rgb, 0, 255, 136), 0.3);
  margin: 0.5rem 0;
}

@keyframes blink {
  0%, 50% {
    opacity: 1;
  }
  51%, 100% {
    opacity: 0;
  }
}

@keyframes pulse {
  0%, 100% {
    opacity: 0.3;
  }
  50% {
    opacity: 0.6;
  }
}

/* Scrollbar styling */
.panel-body::-webkit-scrollbar {
  width: 8px;
}

.panel-body::-webkit-scrollbar-track {
  background: rgba(0, 0, 0, 0.3);
}

.panel-body::-webkit-scrollbar-thumb {
  background: rgba(var(--crt-primary-rgb, 0, 255, 136), 0.3);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.5);
}

.panel-body::-webkit-scrollbar-thumb:hover {
  background: rgba(var(--crt-primary-rgb, 0, 255, 136), 0.5);
}
</style>
