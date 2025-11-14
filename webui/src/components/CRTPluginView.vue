<script setup lang="ts">
import { ref } from 'vue'
import DigitalSlider from './controls/DigitalSlider.vue'
import DigitalDial from './controls/DigitalDial.vue'
import DigitalToggle from './controls/DigitalToggle.vue'

type ColorTheme = 'green' | 'orange' | 'magenta'

const currentTheme = ref<ColorTheme>('green')
const scanlineEnabled = ref(true)

// Audio parameters
const inputGain = ref(75)
const outputGain = ref(80)
const mix = ref(65)
const frequency = ref(1000)
const resonance = ref(45)
const drive = ref(30)
const attack = ref(25)
const release = ref(60)
const threshold = ref(-12)

// System toggles
const powerOn = ref(true)
const bypassEnabled = ref(false)
const limiterEnabled = ref(true)
const stereoLink = ref(false)

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
</script>

<template>
  <div class="crt-plugin" :class="{ 'scanline-active': scanlineEnabled, [`theme-${currentTheme}`]: true }">
    <div class="scanline-overlay"></div>
    <div class="crt-screen">
      <!-- Header Bar -->
      <div class="crt-header">
        <div class="header-left">
          <div class="terminal-prompt">
            <span class="prompt-symbol">&gt;</span>
            <span class="prompt-text">NODAL_AUDIO_PROCESSOR</span>
            <span class="cursor-blink">_</span>
          </div>
        </div>
        <div class="header-right">
          <div class="system-status">
            <span class="status-label">STATUS:</span>
            <span class="status-value" :class="{ active: powerOn }">
              {{ powerOn ? 'ONLINE' : 'OFFLINE' }}
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
        <!-- Left Panel - Input -->
        <div class="control-panel">
          <div class="panel-header">
            <div class="panel-title">// INPUT_STAGE</div>
            <div class="panel-line"></div>
          </div>
          <div class="panel-body">
            <DigitalSlider 
              v-model="inputGain" 
              label="INPUT_GAIN"
              :min="0"
              :max="100"
              :color="themeColors[currentTheme]"
            />
            <DigitalSlider 
              v-model="drive" 
              label="DRIVE"
              :min="0"
              :max="100"
              :color="themeColors[currentTheme]"
            />
            <div class="toggle-group">
              <DigitalToggle 
                v-model="bypassEnabled" 
                label="BYPASS"
                size="medium"
                :colorOn="themeColors[currentTheme]"
              />
              <DigitalToggle 
                v-model="stereoLink" 
                label="STEREO"
                size="medium"
                :colorOn="themeColors[currentTheme]"
              />
            </div>
          </div>
        </div>

        <!-- Center Panel - Processing -->
        <div class="control-panel center-panel">
          <div class="panel-header">
            <div class="panel-title">// PROCESSING_CORE</div>
            <div class="panel-line"></div>
          </div>
          <div class="panel-body">
            <div class="dial-row">
              <DigitalDial 
                v-model="frequency" 
                label="FREQ"
                :min="20"
                :max="20000"
                :step="10"
                :size="110"
                :segments="20"
                :color="themeColors[currentTheme]"
              />
              <DigitalDial 
                v-model="resonance" 
                label="RES"
                :min="0"
                :max="100"
                :size="110"
                :segments="20"
                :color="themeColors[currentTheme]"
              />
            </div>
            <div class="dial-row">
              <DigitalDial 
                v-model="attack" 
                label="ATK"
                :min="0"
                :max="100"
                :size="100"
                :segments="18"
                :color="themeColors[currentTheme]"
              />
              <DigitalDial 
                v-model="release" 
                label="REL"
                :min="0"
                :max="100"
                :size="100"
                :segments="18"
                :color="themeColors[currentTheme]"
              />
            </div>
            <DigitalSlider 
              v-model="mix" 
              label="DRY_WET"
              :min="0"
              :max="100"
              :color="themeColors[currentTheme]"
            />
          </div>
        </div>

        <!-- Right Panel - Output -->
        <div class="control-panel">
          <div class="panel-header">
            <div class="panel-title">// OUTPUT_STAGE</div>
            <div class="panel-line"></div>
          </div>
          <div class="panel-body">
            <DigitalSlider 
              v-model="outputGain" 
              label="OUTPUT_GAIN"
              :min="0"
              :max="100"
              :color="themeColors[currentTheme]"
            />
            <DigitalSlider 
              v-model="threshold" 
              label="LIMITER"
              :min="-60"
              :max="0"
              :step="0.5"
              :color="themeColors[currentTheme]"
            />
            <div class="toggle-group">
              <DigitalToggle 
                v-model="limiterEnabled" 
                label="LIMIT"
                size="medium"
                :colorOn="themeColors[currentTheme]"
              />
              <DigitalToggle 
                v-model="powerOn" 
                label="POWER"
                size="medium"
                :colorOn="themeColors[currentTheme]"
              />
            </div>
          </div>
        </div>
      </div>

      <!-- Status Bar -->
      <div class="crt-footer">
        <div class="footer-section">
          <span class="footer-label">CPU:</span>
          <div class="meter-bar">
            <div class="meter-fill" style="width: 42%"></div>
          </div>
          <span class="footer-value">42%</span>
        </div>
        <div class="footer-section">
          <span class="footer-label">BUFFER:</span>
          <span class="footer-value">512</span>
        </div>
        <div class="footer-section">
          <span class="footer-label">RATE:</span>
          <span class="footer-value">48kHz</span>
        </div>
        <div class="footer-section">
          <span class="footer-label">LATENCY:</span>
          <span class="footer-value">10.7ms</span>
        </div>
      </div>
    </div>

    <!-- CRT Effects -->
    <div class="crt-curvature"></div>
    <div class="crt-vignette"></div>
  </div>
</template>

<style scoped>
.crt-plugin {
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
  color: #666;
  font-weight: 700;
  transition: all 0.3s ease;
}

.status-value.active {
  color: var(--crt-primary);
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
  grid-template-columns: 1fr 1.5fr 1fr;
  gap: 1.5rem;
  overflow-y: auto;
}

.control-panel {
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
}

.panel-header {
  padding: 1rem 1.5rem;
  border-bottom: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.2);
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
  display: flex;
  flex-direction: column;
  gap: 2rem;
  justify-content: center;
}

.center-panel .panel-body {
  align-items: center;
}

.dial-row {
  display: flex;
  gap: 2rem;
  justify-content: center;
}

.toggle-group {
  display: flex;
  gap: 1.5rem;
  justify-content: center;
  flex-wrap: wrap;
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

.meter-bar {
  width: 60px;
  height: 8px;
  background: rgba(0, 0, 0, 0.5);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.3);
  position: relative;
  overflow: hidden;
}

.meter-fill {
  position: absolute;
  height: 100%;
  background: var(--crt-primary);
  box-shadow: 0 0 10px var(--crt-primary);
  transition: width 0.3s ease;
}

@keyframes blink {
  0%, 50% {
    opacity: 1;
  }
  51%, 100% {
    opacity: 0;
  }
}

/* Responsive */
@media (max-width: 1200px) {
  .crt-content {
    grid-template-columns: 1fr;
    gap: 1rem;
  }
  
  .dial-row {
    flex-wrap: wrap;
  }
}

/* Glow effect on theme change */
.crt-plugin {
  transition: --crt-primary 0.3s ease;
}
</style>
