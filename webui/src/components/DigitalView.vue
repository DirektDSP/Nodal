<script setup lang="ts">
import { ref } from 'vue'
import DigitalSlider from './controls/DigitalSlider.vue'
import DigitalDial from './controls/DigitalDial.vue'
import DigitalToggle from './controls/DigitalToggle.vue'

// Demo values
const gain = ref(75)
const frequency = ref(1000)
const resonance = ref(50)
const attack = ref(25)
const release = ref(60)
const threshold = ref(-12)

const powerOn = ref(true)
const filterEnabled = ref(false)
const limiterActive = ref(true)
const stereoLink = ref(false)
</script>

<template>
  <main class="digital-view">
    <div class="content-area">
      <div class="digital-showcase">
        <div class="showcase-header">
          <div class="header-frame">
            <div class="corner corner-tl"></div>
            <div class="corner corner-tr"></div>
            <div class="corner corner-bl"></div>
            <div class="corner corner-br"></div>
          </div>
          <h1 class="glitch" data-text="DIGITAL CONTROLS">DIGITAL CONTROLS</h1>
          <p class="subheading">// ANGULAR_DESIGN_SYSTEM</p>
          <div class="scanline"></div>
        </div>

        <!-- Sliders Section -->
        <div class="control-section">
          <div class="section-header">
            <h2 class="section-title">&gt; LINEAR_FADERS</h2>
            <div class="section-line"></div>
          </div>
          <div class="controls-grid">
            <DigitalSlider 
              v-model="gain" 
              label="GAIN"
              :min="0"
              :max="100"
              color="#00ff88"
            />
            <DigitalSlider 
              v-model="frequency" 
              label="FREQUENCY"
              :min="20"
              :max="20000"
              :step="10"
              color="#00d4ff"
            />
            <DigitalSlider 
              v-model="threshold" 
              label="THRESHOLD"
              :min="-60"
              :max="0"
              :step="0.5"
              color="#ff0080"
            />
          </div>
        </div>

        <!-- Dials Section -->
        <div class="control-section">
          <div class="section-header">
            <h2 class="section-title">&gt; ROTARY_ENCODERS</h2>
            <div class="section-line"></div>
          </div>
          <div class="dials-grid">
            <DigitalDial 
              v-model="resonance" 
              label="RESONANCE"
              :min="0"
              :max="100"
              :size="110"
              :segments="20"
              color="#00ff88"
            />
            <DigitalDial 
              v-model="attack" 
              label="ATTACK"
              :min="0"
              :max="100"
              :size="130"
              :segments="24"
              color="#ffaa00"
            />
            <DigitalDial 
              v-model="release" 
              label="RELEASE"
              :min="0"
              :max="100"
              :size="110"
              :segments="20"
              color="#ff0080"
            />
          </div>
        </div>

        <!-- Toggles Section -->
        <div class="control-section">
          <div class="section-header">
            <h2 class="section-title">&gt; BINARY_SWITCHES</h2>
            <div class="section-line"></div>
          </div>
          <div class="toggles-grid">
            <DigitalToggle 
              v-model="powerOn" 
              label="POWER"
              size="medium"
              colorOn="#00ff88"
            />
            <DigitalToggle 
              v-model="filterEnabled" 
              label="FILTER"
              size="medium"
              colorOn="#00d4ff"
            />
            <DigitalToggle 
              v-model="limiterActive" 
              label="LIMITER"
              size="medium"
              colorOn="#ffaa00"
            />
            <DigitalToggle 
              v-model="stereoLink" 
              label="STEREO_LINK"
              size="medium"
              colorOn="#ff0080"
            />
          </div>
        </div>

        <!-- Command Panel -->
        <div class="control-section command-panel">
          <div class="section-header">
            <h2 class="section-title">&gt; COMMAND_PANEL</h2>
            <div class="section-line"></div>
          </div>
          <div class="panel-layout">
            <div class="panel-left">
              <DigitalSlider 
                v-model="gain" 
                label="MASTER_GAIN"
                :min="0"
                :max="100"
                color="#00ff88"
              />
              <DigitalSlider 
                v-model="threshold" 
                label="LIMIT_THRESH"
                :min="-60"
                :max="0"
                :step="0.5"
                color="#ff0080"
              />
              <div class="toggles-row">
                <DigitalToggle 
                  v-model="powerOn" 
                  label="PWR"
                  size="large"
                  colorOn="#00ff88"
                />
                <DigitalToggle 
                  v-model="limiterActive" 
                  label="LIM"
                  size="large"
                  colorOn="#ffaa00"
                />
              </div>
            </div>
            <div class="panel-center">
              <div class="dials-row">
                <DigitalDial 
                  v-model="attack" 
                  label="ATK"
                  :min="0"
                  :max="100"
                  :size="100"
                  :segments="18"
                  color="#ffaa00"
                />
                <DigitalDial 
                  v-model="release" 
                  label="REL"
                  :min="0"
                  :max="100"
                  :size="100"
                  :segments="18"
                  color="#ff0080"
                />
              </div>
            </div>
            <div class="panel-right">
              <div class="status-display">
                <div class="status-line">
                  <span class="label">STATUS:</span>
                  <span class="value" :class="{ active: powerOn }">
                    {{ powerOn ? 'ONLINE' : 'OFFLINE' }}
                  </span>
                </div>
                <div class="status-line">
                  <span class="label">GAIN:</span>
                  <span class="value">{{ gain.toFixed(1) }}%</span>
                </div>
                <div class="status-line">
                  <span class="label">FREQ:</span>
                  <span class="value">{{ frequency }}Hz</span>
                </div>
                <div class="status-line">
                  <span class="label">SYSTEMS:</span>
                  <span class="value active">NOMINAL</span>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </main>
</template>

<style scoped>
.digital-view {
  flex: 1;
  display: flex;
  flex-direction: column;
  background: var(--bg-primary);
  overflow-y: auto;
  font-family: 'Courier New', monospace;
}

.content-area {
  flex: 1;
  padding: 2rem;
}

.digital-showcase {
  max-width: 1400px;
  margin: 0 auto;
}

.showcase-header {
  position: relative;
  text-align: center;
  margin-bottom: 3rem;
  padding: 3rem 2rem;
  background: linear-gradient(135deg, var(--bg-secondary) 0%, var(--bg-tertiary) 100%);
  clip-path: polygon(
    20px 0, calc(100% - 20px) 0, 100% 20px, 
    100% calc(100% - 20px), calc(100% - 20px) 100%, 
    20px 100%, 0 calc(100% - 20px), 0 20px
  );
  box-shadow: 
    0 0 30px rgba(0, 255, 136, 0.2),
    inset 0 0 30px rgba(0, 0, 0, 0.3);
  overflow: hidden;
}

.header-frame {
  position: absolute;
  inset: 10px;
  pointer-events: none;
  z-index: 1;
}

.corner {
  position: absolute;
  width: 20px;
  height: 20px;
  border: 2px solid rgba(0, 255, 136, 0.5);
}

.corner-tl {
  top: 0;
  left: 0;
  border-right: none;
  border-bottom: none;
  box-shadow: 0 0 10px rgba(0, 255, 136, 0.3);
}

.corner-tr {
  top: 0;
  right: 0;
  border-left: none;
  border-bottom: none;
  box-shadow: 0 0 10px rgba(0, 255, 136, 0.3);
}

.corner-bl {
  bottom: 0;
  left: 0;
  border-right: none;
  border-top: none;
  box-shadow: 0 0 10px rgba(0, 255, 136, 0.3);
}

.corner-br {
  bottom: 0;
  right: 0;
  border-left: none;
  border-top: none;
  box-shadow: 0 0 10px rgba(0, 255, 136, 0.3);
}

.glitch {
  font-size: 3rem;
  font-weight: 900;
  margin-bottom: 0.5rem;
  color: #00ff88;
  text-transform: uppercase;
  letter-spacing: 0.2em;
  position: relative;
  text-shadow: 
    0 0 10px rgba(0, 255, 136, 0.5),
    0 0 20px rgba(0, 255, 136, 0.3),
    0 0 30px rgba(0, 255, 136, 0.2);
  animation: glitch 3s infinite;
}

.glitch::before,
.glitch::after {
  content: attr(data-text);
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  opacity: 0.8;
}

.glitch::before {
  animation: glitchTop 2s infinite;
  clip-path: polygon(0 0, 100% 0, 100% 33%, 0 33%);
}

.glitch::after {
  animation: glitchBottom 1.5s infinite;
  clip-path: polygon(0 67%, 100% 67%, 100% 100%, 0 100%);
}

.subheading {
  font-size: 1rem;
  color: var(--text-secondary);
  opacity: 0.7;
  letter-spacing: 0.1em;
  font-weight: 700;
}

.scanline {
  position: absolute;
  inset: 0;
  background: linear-gradient(
    to bottom,
    transparent 0%,
    rgba(0, 255, 136, 0.1) 50%,
    transparent 100%
  );
  animation: scanline 4s linear infinite;
  pointer-events: none;
}

.control-section {
  background: var(--bg-secondary);
  margin-bottom: 2rem;
  padding: 2rem;
  clip-path: polygon(
    15px 0, calc(100% - 15px) 0, 100% 15px, 
    100% calc(100% - 15px), calc(100% - 15px) 100%, 
    15px 100%, 0 calc(100% - 15px), 0 15px
  );
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
  border: 1px solid var(--border-color);
  position: relative;
}

.section-header {
  margin-bottom: 2rem;
  position: relative;
}

.section-title {
  font-size: 1rem;
  font-weight: 700;
  color: #00ff88;
  text-transform: uppercase;
  letter-spacing: 0.15em;
  opacity: 0.9;
  text-shadow: 0 0 10px rgba(0, 255, 136, 0.3);
}

.section-line {
  height: 2px;
  background: linear-gradient(90deg, 
    #00ff88 0%, 
    transparent 50%
  );
  margin-top: 0.5rem;
  box-shadow: 0 0 5px rgba(0, 255, 136, 0.3);
}

.controls-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
  gap: 2.5rem;
}

.dials-grid {
  display: flex;
  gap: 3rem;
  justify-content: space-around;
  flex-wrap: wrap;
  padding: 1rem 0;
}

.toggles-grid {
  display: flex;
  gap: 2.5rem;
  flex-wrap: wrap;
}

.command-panel {
  background: linear-gradient(135deg, var(--bg-secondary) 0%, rgba(0, 255, 136, 0.03) 100%);
  border: 1px solid rgba(0, 255, 136, 0.3);
  box-shadow: 
    0 0 20px rgba(0, 255, 136, 0.2),
    inset 0 0 30px rgba(0, 0, 0, 0.3);
}

.panel-layout {
  display: grid;
  grid-template-columns: 1fr auto 1fr;
  gap: 3rem;
  align-items: start;
}

.panel-left {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.panel-center {
  display: flex;
  align-items: center;
  justify-content: center;
}

.panel-right {
  display: flex;
  align-items: center;
  justify-content: center;
}

.dials-row {
  display: flex;
  gap: 2rem;
  justify-content: center;
}

.toggles-row {
  display: flex;
  gap: 1.5rem;
  justify-content: center;
  margin-top: 1rem;
}

.status-display {
  background: rgba(0, 0, 0, 0.5);
  padding: 1.5rem;
  clip-path: polygon(
    10px 0, calc(100% - 10px) 0, 100% 10px, 
    100% calc(100% - 10px), calc(100% - 10px) 100%, 
    10px 100%, 0 calc(100% - 10px), 0 10px
  );
  border: 1px solid rgba(0, 255, 136, 0.3);
  min-width: 250px;
  box-shadow: 
    inset 0 0 20px rgba(0, 0, 0, 0.5),
    0 0 15px rgba(0, 255, 136, 0.2);
}

.status-line {
  display: flex;
  justify-content: space-between;
  gap: 1rem;
  margin-bottom: 1rem;
  padding: 0.5rem;
  border-bottom: 1px solid rgba(0, 255, 136, 0.2);
}

.status-line:last-child {
  border-bottom: none;
  margin-bottom: 0;
}

.status-line .label {
  font-size: 0.75rem;
  color: var(--text-secondary);
  font-weight: 700;
  letter-spacing: 0.1em;
}

.status-line .value {
  font-size: 0.875rem;
  color: var(--text-primary);
  font-weight: 700;
  font-variant-numeric: tabular-nums;
}

.status-line .value.active {
  color: #00ff88;
  text-shadow: 0 0 10px rgba(0, 255, 136, 0.5);
  animation: blink 2s ease-in-out infinite;
}

@keyframes glitch {
  0%, 90%, 100% {
    transform: translate(0);
  }
  91% {
    transform: translate(-2px, 2px);
  }
  92% {
    transform: translate(2px, -2px);
  }
  93% {
    transform: translate(-2px, 2px);
  }
}

@keyframes glitchTop {
  0%, 90%, 100% {
    transform: translate(0);
  }
  91% {
    transform: translate(-3px, 0);
    text-shadow: 3px 0 #ff00ff;
  }
}

@keyframes glitchBottom {
  0%, 90%, 100% {
    transform: translate(0);
  }
  91% {
    transform: translate(3px, 0);
    text-shadow: -3px 0 #00ffff;
  }
}

@keyframes scanline {
  0% {
    transform: translateY(-100%);
  }
  100% {
    transform: translateY(100%);
  }
}

@keyframes blink {
  0%, 100% {
    opacity: 1;
  }
  50% {
    opacity: 0.6;
  }
}

/* Responsive */
@media (max-width: 1024px) {
  .panel-layout {
    grid-template-columns: 1fr;
    gap: 2rem;
  }
  
  .panel-center,
  .panel-right {
    justify-content: flex-start;
  }
}

@media (max-width: 768px) {
  .content-area {
    padding: 1rem;
  }
  
  .glitch {
    font-size: 2rem;
  }
  
  .controls-grid {
    grid-template-columns: 1fr;
  }
  
  .dials-grid {
    gap: 2rem;
  }
  
  .control-section {
    padding: 1.5rem;
  }
}
</style>
