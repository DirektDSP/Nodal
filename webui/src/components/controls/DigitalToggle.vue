<script setup lang="ts">
import { ref } from 'vue'

interface Props {
  modelValue: boolean
  label?: string
  colorOn?: string
  colorOff?: string
  size?: 'small' | 'medium' | 'large'
}

const props = withDefaults(defineProps<Props>(), {
  label: '',
  colorOn: '#00ff88',
  colorOff: '#4b5563',
  size: 'medium'
})

const emit = defineEmits<{
  'update:modelValue': [value: boolean]
}>()

const isHovering = ref(false)

const toggle = () => {
  emit('update:modelValue', !props.modelValue)
}

const sizeMap = {
  small: { width: 50, height: 24 },
  medium: { width: 60, height: 28 },
  large: { width: 70, height: 32 }
}
</script>

<template>
  <div class="digital-toggle-container">
    <label 
      class="toggle-wrapper"
      :class="[`toggle-${size}`, { 'is-hovering': isHovering, 'is-on': modelValue }]"
      @mouseenter="isHovering = true"
      @mouseleave="isHovering = false"
    >
      <input 
        type="checkbox" 
        :checked="modelValue"
        @change="toggle"
        class="toggle-input"
      />
      
      <div 
        class="toggle-track"
        :style="{ 
          '--color-on': colorOn,
          '--color-off': colorOff,
          width: `${sizeMap[size].width}px`,
          height: `${sizeMap[size].height}px`
        }"
      >
        <div class="track-frame">
          <div class="track-corner track-tl"></div>
          <div class="track-corner track-tr"></div>
          <div class="track-corner track-bl"></div>
          <div class="track-corner track-br"></div>
        </div>
        
        <div class="track-grid">
          <div class="grid-cell" v-for="i in 6" :key="i"></div>
        </div>
        
        <div class="track-glow"></div>
        
        <div class="toggle-thumb">
          <div class="thumb-body">
            <div class="thumb-lines">
              <div class="line" v-for="i in 3" :key="i"></div>
            </div>
          </div>
          <div class="thumb-glow"></div>
        </div>
        
        <div class="track-indicator">
          <span class="indicator-text off">OFF</span>
          <span class="indicator-text on">ON</span>
        </div>
      </div>
      
      <span v-if="label" class="toggle-label">{{ label }}</span>
    </label>
  </div>
</template>

<style scoped>
.digital-toggle-container {
  display: inline-flex;
  padding: 0.5rem 0;
  font-family: 'Courier New', monospace;
}

.toggle-wrapper {
  display: inline-flex;
  align-items: center;
  gap: 1rem;
  cursor: pointer;
  user-select: none;
  transition: transform 0.2s ease;
}

.toggle-wrapper.is-hovering {
  transform: scale(1.05);
}

.toggle-input {
  position: absolute;
  opacity: 0;
  pointer-events: none;
  width: 0;
  height: 0;
}

.toggle-track {
  position: relative;
  background: var(--color-off);
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  box-shadow: 
    inset 0 0 10px rgba(0, 0, 0, 0.5),
    inset 0 2px 6px rgba(0, 0, 0, 0.4);
  border: none;
  outline: none;
  overflow: hidden;
  clip-path: polygon(4px 0, calc(100% - 4px) 0, 100% 4px, 100% calc(100% - 4px), calc(100% - 4px) 100%, 4px 100%, 0 calc(100% - 4px), 0 4px);
}

.toggle-wrapper.is-on .toggle-track {
  background: var(--color-on);
  box-shadow: 
    inset 0 0 10px rgba(0, 0, 0, 0.3),
    0 0 15px var(--color-on),
    0 0 25px rgba(0, 255, 136, 0.3);
}

.toggle-wrapper.is-hovering.is-on .toggle-track {
  box-shadow: 
    inset 0 0 10px rgba(0, 0, 0, 0.3),
    0 0 20px var(--color-on),
    0 0 35px var(--color-on),
    0 0 50px rgba(0, 255, 136, 0.2);
}

.track-frame {
  position: absolute;
  inset: 2px;
  pointer-events: none;
  z-index: 5;
}

.track-corner {
  position: absolute;
  width: 6px;
  height: 6px;
  border: 1px solid rgba(255, 255, 255, 0.2);
}

.track-tl {
  top: 0;
  left: 0;
  border-right: none;
  border-bottom: none;
}

.track-tr {
  top: 0;
  right: 0;
  border-left: none;
  border-bottom: none;
}

.track-bl {
  bottom: 0;
  left: 0;
  border-right: none;
  border-top: none;
}

.track-br {
  bottom: 0;
  right: 0;
  border-left: none;
  border-top: none;
}

.track-grid {
  position: absolute;
  inset: 4px;
  display: flex;
  gap: 1px;
  opacity: 0.2;
}

.grid-cell {
  flex: 1;
  border-right: 1px solid rgba(255, 255, 255, 0.3);
}

.grid-cell:last-child {
  border-right: none;
}

.track-glow {
  position: absolute;
  inset: -4px;
  background: var(--color-on);
  opacity: 0;
  filter: blur(15px);
  transition: opacity 0.3s ease;
  pointer-events: none;
}

.toggle-wrapper.is-on .track-glow {
  opacity: 0.3;
  animation: trackPulse 2s ease-in-out infinite;
}

.toggle-thumb {
  position: absolute;
  top: 2px;
  left: 2px;
  width: calc(50% - 2px);
  bottom: 2px;
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  z-index: 3;
}

.toggle-wrapper.is-on .toggle-thumb {
  left: calc(50%);
}

.thumb-body {
  position: relative;
  width: 100%;
  height: 100%;
  background: linear-gradient(135deg, #f0f0f0 0%, #c0c0c0 100%);
  clip-path: polygon(3px 0, calc(100% - 3px) 0, 100% 3px, 100% calc(100% - 3px), calc(100% - 3px) 100%, 3px 100%, 0 calc(100% - 3px), 0 3px);
  box-shadow: 
    0 2px 6px rgba(0, 0, 0, 0.4),
    inset 0 1px 2px rgba(255, 255, 255, 0.5);
  display: flex;
  align-items: center;
  justify-content: center;
}

.toggle-wrapper.is-on .thumb-body {
  box-shadow: 
    0 2px 8px rgba(0, 0, 0, 0.4),
    0 0 10px var(--color-on),
    inset 0 1px 2px rgba(255, 255, 255, 0.5);
}

.thumb-lines {
  display: flex;
  flex-direction: column;
  gap: 2px;
  align-items: center;
}

.line {
  width: 60%;
  height: 1px;
  background: rgba(0, 0, 0, 0.3);
}

.thumb-glow {
  position: absolute;
  inset: -50%;
  background: var(--color-on);
  opacity: 0;
  filter: blur(10px);
  transition: opacity 0.3s ease;
  pointer-events: none;
}

.toggle-wrapper.is-on .thumb-glow {
  opacity: 0.5;
  animation: thumbPulse 2s ease-in-out infinite;
}

.track-indicator {
  position: absolute;
  inset: 0;
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0 6px;
  pointer-events: none;
  z-index: 2;
}

.indicator-text {
  font-size: 0.5rem;
  font-weight: 700;
  letter-spacing: 0.05em;
  transition: all 0.3s ease;
  text-shadow: 0 0 5px currentColor;
}

.indicator-text.off {
  color: rgba(255, 255, 255, 0.8);
  opacity: 1;
}

.indicator-text.on {
  color: rgba(255, 255, 255, 0.3);
  opacity: 0.5;
}

.toggle-wrapper.is-on .indicator-text.off {
  color: rgba(255, 255, 255, 0.3);
  opacity: 0.5;
}

.toggle-wrapper.is-on .indicator-text.on {
  color: rgba(255, 255, 255, 0.9);
  opacity: 1;
  text-shadow: 0 0 8px currentColor;
}

.toggle-label {
  font-size: 0.75rem;
  font-weight: 700;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.15em;
  transition: color 0.3s ease;
}

.toggle-wrapper.is-on .toggle-label {
  color: var(--text-primary);
}

@keyframes trackPulse {
  0%, 100% {
    opacity: 0.3;
  }
  50% {
    opacity: 0.5;
  }
}

@keyframes thumbPulse {
  0%, 100% {
    opacity: 0.4;
  }
  50% {
    opacity: 0.7;
  }
}
</style>
