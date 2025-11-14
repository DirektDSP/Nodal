<script setup lang="ts">
import { ref } from 'vue'
import DigitalToggle from './controls/DigitalToggle.vue'

type ColorTheme = 'green' | 'orange' | 'magenta'

const currentTheme = ref<ColorTheme>('green')
const scanlineEnabled = ref(true)

const bufferSize = ref(512)
const sampleRate = ref(48000)
const pluginVersion = ref('1.0.0')
const license = ref('MIT License')

// Toggle states
const autoSave = ref(false)
const showTooltips = ref(true)
const highQualityRendering = ref(true)

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
  <div class="crt-settings" :class="{ 'scanline-active': scanlineEnabled, [`theme-${currentTheme}`]: true }">
    <div class="scanline-overlay"></div>
    <div class="crt-screen">
      <!-- Header Bar -->
      <div class="crt-header">
        <div class="header-left">
          <div class="terminal-prompt">
            <span class="prompt-symbol">&gt;</span>
            <span class="prompt-text">SYSTEM_CONFIGURATION</span>
            <span class="cursor-blink">_</span>
          </div>
        </div>
        <div class="header-right">
          <div class="system-status">
            <span class="status-label">VERSION:</span>
            <span class="status-value active">{{ pluginVersion }}</span>
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

      <!-- Main Content -->
      <div class="crt-content">
        <div class="settings-panel">
          <!-- Audio Settings -->
          <div class="panel-section">
            <div class="panel-header">
              <div class="panel-title">// AUDIO_CONFIGURATION</div>
              <div class="panel-line"></div>
            </div>
            <div class="panel-body">
              <div class="setting-row">
                <label class="setting-label">BUFFER_SIZE</label>
                <select v-model="bufferSize" class="setting-select">
                  <option :value="256">256_SAMPLES</option>
                  <option :value="512">512_SAMPLES</option>
                  <option :value="1024">1024_SAMPLES</option>
                  <option :value="2048">2048_SAMPLES</option>
                </select>
              </div>
              
              <div class="setting-row">
                <label class="setting-label">SAMPLE_RATE</label>
                <select v-model="sampleRate" class="setting-select">
                  <option :value="44100">44.1_KHZ</option>
                  <option :value="48000">48_KHZ</option>
                  <option :value="88200">88.2_KHZ</option>
                  <option :value="96000">96_KHZ</option>
                </select>
              </div>
            </div>
          </div>

          <!-- System Information -->
          <div class="panel-section">
            <div class="panel-header">
              <div class="panel-title">// SYSTEM_INFO</div>
              <div class="panel-line"></div>
            </div>
            <div class="panel-body">
              <div class="info-row">
                <span class="info-label">VERSION:</span>
                <span class="info-value">{{ pluginVersion }}</span>
              </div>
              <div class="info-row">
                <span class="info-label">LICENSE:</span>
                <span class="info-value">{{ license }}</span>
              </div>
              <div class="info-row">
                <span class="info-label">DEVELOPER:</span>
                <span class="info-value">DIREKT_DSP</span>
              </div>
              <div class="info-row">
                <span class="info-label">BUILD:</span>
                <span class="info-value">2025.11.14</span>
              </div>
            </div>
          </div>

          <!-- Preferences -->
          <div class="panel-section">
            <div class="panel-header">
              <div class="panel-title">// PREFERENCES</div>
              <div class="panel-line"></div>
            </div>
            <div class="panel-body">
              <div class="toggles-grid">
                <DigitalToggle 
                  v-model="autoSave" 
                  label="AUTO_SAVE"
                  size="medium"
                  :colorOn="themeColors[currentTheme]"
                />
                <DigitalToggle 
                  v-model="showTooltips" 
                  label="TOOLTIPS"
                  size="medium"
                  :colorOn="themeColors[currentTheme]"
                />
                <DigitalToggle 
                  v-model="highQualityRendering" 
                  label="HQ_RENDER"
                  size="medium"
                  :colorOn="themeColors[currentTheme]"
                />
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- Status Bar -->
      <div class="crt-footer">
        <div class="footer-section">
          <span class="footer-label">BUFFER:</span>
          <span class="footer-value">{{ bufferSize }}</span>
        </div>
        <div class="footer-section">
          <span class="footer-label">RATE:</span>
          <span class="footer-value">{{ sampleRate / 1000 }}kHz</span>
        </div>
        <div class="footer-section">
          <span class="footer-label">LATENCY:</span>
          <span class="footer-value">{{ ((bufferSize / sampleRate) * 1000).toFixed(1) }}ms</span>
        </div>
        <div class="footer-section">
          <span class="footer-label">THEME:</span>
          <span class="footer-value">{{ currentTheme.toUpperCase() }}</span>
        </div>
      </div>
    </div>

    <!-- CRT Effects -->
    <div class="crt-curvature"></div>
    <div class="crt-vignette"></div>
  </div>
</template>

<style scoped>
.crt-settings {
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
  overflow-y: auto;
  display: flex;
  justify-content: center;
  padding: 1rem;
}

.settings-panel {
  width: 100%;
  max-width: 900px;
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.panel-section {
  background: rgba(0, 0, 0, 0.6);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.2);
  clip-path: polygon(
    15px 0, calc(100% - 15px) 0, 100% 15px,
    100% calc(100% - 15px), calc(100% - 15px) 100%,
    15px 100%, 0 calc(100% - 15px), 0 15px
  );
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
  padding: 1.5rem;
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.setting-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 2rem;
}

.setting-label {
  font-size: 0.875rem;
  font-weight: 700;
  color: var(--crt-primary);
  text-shadow: 0 0 5px var(--crt-primary);
  letter-spacing: 0.1em;
}

.setting-select {
  background: rgba(0, 0, 0, 0.8);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.3);
  color: var(--crt-primary);
  font-family: 'Courier New', monospace;
  font-size: 0.875rem;
  font-weight: 700;
  padding: 0.5rem 1rem;
  min-width: 200px;
  cursor: pointer;
  clip-path: polygon(8px 0, calc(100% - 8px) 0, 100% 8px, 100% calc(100% - 8px), calc(100% - 8px) 100%, 8px 100%, 0 calc(100% - 8px), 0 8px);
  text-shadow: 0 0 5px var(--crt-primary);
  transition: all 0.2s ease;
}

.setting-select:hover {
  border-color: var(--crt-primary);
  box-shadow: 0 0 15px rgba(var(--crt-primary-rgb, 0, 255, 136), 0.3);
}

.setting-select:focus {
  outline: none;
  border-color: var(--crt-primary);
  box-shadow: 0 0 20px rgba(var(--crt-primary-rgb, 0, 255, 136), 0.5);
}

.setting-select option {
  background: #0a0a0a;
  color: var(--crt-primary);
}

.info-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-size: 0.875rem;
  padding: 0.75rem 1rem;
  background: rgba(0, 0, 0, 0.3);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.1);
  clip-path: polygon(6px 0, calc(100% - 6px) 0, 100% 6px, 100% calc(100% - 6px), calc(100% - 6px) 100%, 6px 100%, 0 calc(100% - 6px), 0 6px);
}

.info-label {
  color: var(--text-secondary);
  font-weight: 700;
  letter-spacing: 0.1em;
}

.info-value {
  color: var(--crt-primary);
  font-weight: 700;
  text-shadow: 0 0 5px var(--crt-primary);
}

.toggles-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
  gap: 2rem;
  justify-items: center;
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

@keyframes blink {
  0%, 50% {
    opacity: 1;
  }
  51%, 100% {
    opacity: 0;
  }
}

/* Scrollbar styling */
.crt-content::-webkit-scrollbar {
  width: 10px;
}

.crt-content::-webkit-scrollbar-track {
  background: rgba(0, 0, 0, 0.3);
}

.crt-content::-webkit-scrollbar-thumb {
  background: rgba(var(--crt-primary-rgb, 0, 255, 136), 0.3);
  border: 1px solid rgba(var(--crt-primary-rgb, 0, 255, 136), 0.5);
  clip-path: polygon(3px 0, calc(100% - 3px) 0, 100% 3px, 100% calc(100% - 3px), calc(100% - 3px) 100%, 3px 100%, 0 calc(100% - 3px), 0 3px);
}

.crt-content::-webkit-scrollbar-thumb:hover {
  background: rgba(var(--crt-primary-rgb, 0, 255, 136), 0.5);
}

.settings-title {
  font-size: 2.5rem;
  font-weight: 300;
  margin-bottom: 2.5rem;
  color: var(--text-primary);
}

.settings-section {
  margin-bottom: 3rem;
  padding-bottom: 2rem;
  border-bottom: 1px solid var(--border-color);
}

.settings-section:last-child {
  border-bottom: none;
}

.settings-section h2 {
  font-size: 1.5rem;
  font-weight: 500;
  margin-bottom: 1.5rem;
  color: var(--text-primary);
}

.setting-item {
  margin-bottom: 1.5rem;
}

.setting-item label {
  display: block;
  margin-bottom: 0.5rem;
  font-weight: 500;
  color: var(--text-primary);
  font-size: 0.95rem;
}

.setting-item select {
  width: 100%;
  padding: 0.75rem 1rem;
  background: var(--bg-secondary);
  border: 1px solid var(--border-color);
  border-radius: 6px;
  color: var(--text-primary);
  font-size: 1rem;
  cursor: pointer;
  transition: all 0.2s ease;
}

.setting-item select:hover {
  border-color: var(--accent-primary);
}

.setting-item select:focus {
  outline: none;
  border-color: var(--accent-primary);
  box-shadow: 0 0 0 3px rgba(99, 102, 241, 0.1);
}

.checkbox-label {
  display: flex;
  align-items: center;
  cursor: pointer;
  user-select: none;
}

.checkbox-label input[type="checkbox"] {
  width: 20px;
  height: 20px;
  margin-right: 0.75rem;
  cursor: pointer;
  accent-color: var(--accent-primary);
}

.checkbox-label span {
  color: var(--text-primary);
  font-size: 1rem;
}

.toggles-group {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.info-item {
  display: flex;
  justify-content: space-between;
  padding: 0.75rem 0;
}

.info-label {
  font-weight: 500;
  color: var(--text-secondary);
}

.info-value {
  color: var(--text-primary);
}
</style>
