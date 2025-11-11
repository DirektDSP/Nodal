<script setup lang="ts">
import { ref } from 'vue'

const bufferSize = ref(512)
const sampleRate = ref(48000)
const pluginVersion = ref('1.0.0')
const license = ref('MIT License')
</script>

<template>
  <main class="settings-view">
    <div class="settings-container">
      <h1 class="settings-title">Settings</h1>
      
      <div class="settings-section">
        <h2>Audio Settings</h2>
        <div class="setting-item">
          <label for="buffer-size">Buffer Size</label>
          <select id="buffer-size" v-model="bufferSize">
            <option :value="256">256 samples</option>
            <option :value="512">512 samples</option>
            <option :value="1024">1024 samples</option>
            <option :value="2048">2048 samples</option>
          </select>
        </div>
        
        <div class="setting-item">
          <label for="sample-rate">Sample Rate</label>
          <select id="sample-rate" v-model="sampleRate">
            <option :value="44100">44.1 kHz</option>
            <option :value="48000">48 kHz</option>
            <option :value="88200">88.2 kHz</option>
            <option :value="96000">96 kHz</option>
          </select>
        </div>
      </div>

      <div class="settings-section">
        <h2>Plugin Information</h2>
        <div class="info-item">
          <span class="info-label">Version:</span>
          <span class="info-value">{{ pluginVersion }}</span>
        </div>
        <div class="info-item">
          <span class="info-label">License:</span>
          <span class="info-value">{{ license }}</span>
        </div>
        <div class="info-item">
          <span class="info-label">Developer:</span>
          <span class="info-value">DirektDSP</span>
        </div>
      </div>

      <div class="settings-section">
        <h2>Preferences</h2>
        <div class="setting-item">
          <label class="checkbox-label">
            <input type="checkbox" />
            <span>Enable Auto-Save</span>
          </label>
        </div>
        <div class="setting-item">
          <label class="checkbox-label">
            <input type="checkbox" checked />
            <span>Show Tooltips</span>
          </label>
        </div>
        <div class="setting-item">
          <label class="checkbox-label">
            <input type="checkbox" checked />
            <span>High Quality Rendering</span>
          </label>
        </div>
      </div>
    </div>
  </main>
</template>

<style scoped>
.settings-view {
  flex: 1;
  display: flex;
  background: var(--bg-primary);
  overflow-y: auto;
}

.settings-container {
  flex: 1;
  padding: 3rem;
  max-width: 800px;
  margin: 0 auto;
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
