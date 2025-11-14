<script setup lang="ts">
import { ref } from 'vue'
import GlowSlider from './controls/GlowSlider.vue'
import GlowDial from './controls/GlowDial.vue'
import GlowToggle from './controls/GlowToggle.vue'

// Demo values
const volume = ref(75)
const frequency = ref(440)
const resonance = ref(50)
const dryWet = ref(65)
const filterCutoff = ref(2500)
const drive = ref(30)

const bypassEnabled = ref(false)
const monoMode = ref(true)
const limiterEnabled = ref(true)
const highQuality = ref(false)
</script>

<template>
  <main class="main-view">
    <div class="content-area">
      <div class="controls-showcase">
        <div class="showcase-header">
          <h1>Audio Controls</h1>
          <p>Dynamic, glowing UI components</p>
        </div>

        <!-- Sliders Section -->
        <div class="control-section">
          <h2 class="section-title">Sliders</h2>
          <div class="controls-grid">
            <GlowSlider 
              v-model="volume" 
              label="Volume"
              :min="0"
              :max="100"
              color="#6366f1"
            />
            <GlowSlider 
              v-model="frequency" 
              label="Frequency"
              :min="20"
              :max="20000"
              :step="10"
              color="#8b5cf6"
            />
            <GlowSlider 
              v-model="dryWet" 
              label="Dry/Wet"
              :min="0"
              :max="100"
              color="#ec4899"
            />
          </div>
        </div>

        <!-- Dials Section -->
        <div class="control-section">
          <h2 class="section-title">Rotary Dials</h2>
          <div class="dials-grid">
            <GlowDial 
              v-model="resonance" 
              label="Resonance"
              :min="0"
              :max="100"
              :size="100"
              color="#06b6d4"
              startPosition="bottom"
            />
            <GlowDial 
              v-model="filterCutoff" 
              label="Filter Cutoff"
              :min="20"
              :max="20000"
              :step="10"
              :size="120"
              color="#10b981"
              startPosition="left"
            />
            <GlowDial 
              v-model="drive" 
              label="Drive"
              :min="0"
              :max="100"
              :size="100"
              color="#f59e0b"
              startPosition="top"
            />
          </div>
        </div>

        <!-- Toggles Section -->
        <div class="control-section">
          <h2 class="section-title">Toggles</h2>
          <div class="toggles-grid">
            <GlowToggle 
              v-model="bypassEnabled" 
              label="Bypass"
              size="medium"
              colorOn="#ef4444"
            />
            <GlowToggle 
              v-model="monoMode" 
              label="Mono Mode"
              size="medium"
              colorOn="#6366f1"
            />
            <GlowToggle 
              v-model="limiterEnabled" 
              label="Limiter"
              size="medium"
              colorOn="#10b981"
            />
            <GlowToggle 
              v-model="highQuality" 
              label="High Quality"
              size="medium"
              colorOn="#8b5cf6"
            />
          </div>
        </div>

        <!-- Live Demo Section -->
        <div class="control-section demo-panel">
          <h2 class="section-title">Live Demo Panel</h2>
          <div class="demo-layout">
            <div class="demo-column">
              <GlowSlider 
                v-model="volume" 
                label="Master Volume"
                :min="0"
                :max="100"
                color="#6366f1"
              />
              <GlowSlider 
                v-model="dryWet" 
                label="Mix"
                :min="0"
                :max="100"
                color="#ec4899"
              />
            </div>
            <div class="demo-column">
              <div class="dials-row">
                <GlowDial 
                  v-model="resonance" 
                  label="Q"
                  :min="0"
                  :max="100"
                  :size="90"
                  color="#06b6d4"
                />
                <GlowDial 
                  v-model="drive" 
                  label="Gain"
                  :min="0"
                  :max="100"
                  :size="90"
                  color="#f59e0b"
                />
              </div>
            </div>
            <div class="demo-column toggles-column">
              <GlowToggle 
                v-model="bypassEnabled" 
                label="Bypass"
                size="large"
                colorOn="#ef4444"
              />
              <GlowToggle 
                v-model="highQuality" 
                label="HQ Mode"
                size="large"
                colorOn="#8b5cf6"
              />
            </div>
          </div>
        </div>
      </div>
    </div>
  </main>
</template>

<style scoped>
.main-view {
  flex: 1;
  display: flex;
  flex-direction: column;
  background: var(--bg-primary);
  overflow-y: auto;
}

.content-area {
  flex: 1;
  padding: 2rem;
}

.controls-showcase {
  max-width: 1200px;
  margin: 0 auto;
}

.showcase-header {
  text-align: center;
  margin-bottom: 3rem;
  padding: 2rem;
  background: linear-gradient(135deg, var(--bg-secondary) 0%, var(--bg-tertiary) 100%);
  border-radius: 12px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.3);
}

.showcase-header h1 {
  font-size: 2.5rem;
  font-weight: 300;
  margin-bottom: 0.5rem;
  color: var(--text-primary);
  text-transform: uppercase;
  letter-spacing: 0.1em;
}

.showcase-header p {
  font-size: 1rem;
  color: var(--text-secondary);
  opacity: 0.8;
}

.control-section {
  background: var(--bg-secondary);
  border-radius: 12px;
  padding: 2rem;
  margin-bottom: 2rem;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.3);
  border: 1px solid var(--border-color);
}

.section-title {
  font-size: 1.25rem;
  font-weight: 500;
  color: var(--text-primary);
  margin-bottom: 1.5rem;
  text-transform: uppercase;
  letter-spacing: 0.05em;
  opacity: 0.9;
}

.controls-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
  gap: 2rem;
}

.dials-grid {
  display: flex;
  gap: 3rem;
  justify-content: space-around;
  flex-wrap: wrap;
}

.toggles-grid {
  display: flex;
  gap: 2rem;
  flex-wrap: wrap;
}

.demo-panel {
  background: linear-gradient(135deg, var(--bg-secondary) 0%, rgba(99, 102, 241, 0.05) 100%);
  border: 1px solid rgba(99, 102, 241, 0.2);
}

.demo-layout {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 2rem;
  align-items: start;
}

.demo-column {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.dials-row {
  display: flex;
  gap: 2rem;
  justify-content: center;
  flex-wrap: wrap;
}

.toggles-column {
  justify-content: center;
}

/* Responsive adjustments */
@media (max-width: 768px) {
  .content-area {
    padding: 1rem;
  }
  
  .showcase-header h1 {
    font-size: 1.75rem;
  }
  
  .control-section {
    padding: 1.5rem;
  }
  
  .controls-grid {
    grid-template-columns: 1fr;
  }
  
  .dials-grid {
    gap: 2rem;
  }
  
  .demo-layout {
    grid-template-columns: 1fr;
  }
}
</style>
