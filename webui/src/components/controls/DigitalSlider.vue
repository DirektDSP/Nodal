<script setup lang="ts">
import { ref, computed } from 'vue'

interface Props {
  modelValue: number
  min?: number
  max?: number
  step?: number
  label?: string
  color?: string
}

const props = withDefaults(defineProps<Props>(), {
  min: 0,
  max: 100,
  step: 1,
  label: '',
  color: '#00ff88'
})

const emit = defineEmits<{
  'update:modelValue': [value: number]
}>()

const isDragging = ref(false)
const isHovering = ref(false)

const percentage = computed(() => {
  return ((props.modelValue - props.min) / (props.max - props.min)) * 100
})

const handleInput = (event: Event) => {
  const target = event.target as HTMLInputElement
  emit('update:modelValue', parseFloat(target.value))
}

const handleMouseDown = () => {
  isDragging.value = true
}

const handleMouseUp = () => {
  isDragging.value = false
}

const handleMouseEnter = () => {
  isHovering.value = true
}

const handleMouseLeave = () => {
  isHovering.value = false
}

const handleWheel = (event: WheelEvent) => {
  event.preventDefault()
  
  const direction = event.deltaY > 0 ? -1 : 1
  const newValue = props.modelValue + (direction * props.step)
  
  const clampedValue = Math.max(props.min, Math.min(props.max, newValue))
  
  emit('update:modelValue', clampedValue)
}

// Generate grid segments
const segments = computed(() => {
  return Array.from({ length: 20 }, (_, i) => i)
})
</script>

<template>
  <div class="digital-slider-container">
    <div class="slider-header">
      <label v-if="label" class="slider-label">{{ label }}</label>
      <div class="slider-value">
        <span class="value-display">{{ modelValue.toFixed(1) }}</span>
      </div>
    </div>
    <div 
      class="slider-wrapper"
      :class="{ 'is-dragging': isDragging, 'is-hovering': isHovering }"
      @mouseenter="handleMouseEnter"
      @mouseleave="handleMouseLeave"
      @wheel="handleWheel"
    >
      <div class="slider-frame">
        <div class="corner corner-tl"></div>
        <div class="corner corner-tr"></div>
        <div class="corner corner-bl"></div>
        <div class="corner corner-br"></div>
      </div>
      
      <input
        type="range"
        :value="modelValue"
        :min="min"
        :max="max"
        :step="step"
        class="slider-input"
        @input="handleInput"
        @mousedown="handleMouseDown"
        @mouseup="handleMouseUp"
      />
      
      <div class="slider-track">
        <div class="track-grid">
          <div 
            v-for="i in segments" 
            :key="i" 
            class="grid-segment"
            :class="{ 'active': (i / 20) * 100 < percentage }"
          ></div>
        </div>
        <div 
          class="slider-fill" 
          :style="{ 
            width: `${percentage}%`,
            '--slider-color': color
          }"
        >
          <div class="fill-scanline"></div>
        </div>
      </div>
      
      <div 
        class="slider-thumb" 
        :style="{ 
          left: `${percentage}%`,
          '--slider-color': color
        }"
      >
        <div class="thumb-inner">
          <div class="thumb-line"></div>
          <div class="thumb-glow"></div>
        </div>
      </div>
      
      <div class="slider-indicators">
        <div class="indicator" style="left: 0%">|</div>
        <div class="indicator" style="left: 25%">|</div>
        <div class="indicator" style="left: 50%">|</div>
        <div class="indicator" style="left: 75%">|</div>
        <div class="indicator" style="left: 100%">|</div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.digital-slider-container {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
  padding: 0.5rem 0;
  font-family: 'Courier New', monospace;
}

.slider-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 1rem;
}

.slider-label {
  font-size: 0.75rem;
  font-weight: 700;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.15em;
  transition: color 0.3s ease;
}

.slider-value {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.value-display {
  font-size: 0.875rem;
  font-weight: 700;
  color: v-bind(color);
  background: color-mix(in srgb, v-bind(color) 10%, transparent);
  padding: 0.25rem 0.75rem;
  border: 1px solid color-mix(in srgb, v-bind(color) 30%, transparent);
  clip-path: polygon(0 0, calc(100% - 6px) 0, 100% 6px, 100% 100%, 6px 100%, 0 calc(100% - 6px));
  font-variant-numeric: tabular-nums;
  min-width: 60px;
  text-align: center;
  box-shadow: inset 0 0 10px color-mix(in srgb, v-bind(color) 10%, transparent);
  text-shadow: 0 0 8px color-mix(in srgb, v-bind(color) 50%, transparent);
}

.slider-wrapper {
  position: relative;
  height: 50px;
  display: flex;
  align-items: center;
  padding: 0 10px;
  transition: transform 0.2s ease;
}

.slider-wrapper.is-hovering .slider-label {
  color: var(--text-primary);
}

.slider-frame {
  position: absolute;
  inset: 0;
  pointer-events: none;
}

.corner {
  position: absolute;
  width: 8px;
  height: 8px;
  border: 1px solid var(--border-color);
  transition: border-color 0.3s ease;
}

.slider-wrapper.is-hovering .corner {
  border-color: var(--slider-color, #00ff88);
  box-shadow: 0 0 5px var(--slider-color, #00ff88);
}

.corner-tl {
  top: 0;
  left: 0;
  border-right: none;
  border-bottom: none;
}

.corner-tr {
  top: 0;
  right: 0;
  border-left: none;
  border-bottom: none;
}

.corner-bl {
  bottom: 0;
  left: 0;
  border-right: none;
  border-top: none;
}

.corner-br {
  bottom: 0;
  right: 0;
  border-left: none;
  border-top: none;
}

.slider-input {
  position: absolute;
  width: 100%;
  height: 100%;
  opacity: 0;
  cursor: pointer;
  z-index: 10;
}

.slider-track {
  position: relative;
  width: 100%;
  height: 8px;
  background: var(--bg-tertiary);
  clip-path: polygon(4px 0, calc(100% - 4px) 0, 100% 4px, 100% 100%, calc(100% - 4px) 100%, 4px 100%, 0 calc(100% - 4px), 0 4px);
  overflow: hidden;
  box-shadow: 
    inset 0 0 10px rgba(0, 0, 0, 0.5),
    inset 0 2px 4px rgba(0, 0, 0, 0.3);
}

.track-grid {
  position: absolute;
  inset: 0;
  display: flex;
  gap: 2px;
}

.grid-segment {
  flex: 1;
  background: rgba(255, 255, 255, 0.02);
  border-right: 1px solid rgba(255, 255, 255, 0.05);
  transition: background 0.1s ease;
}

.grid-segment.active {
  background: rgba(0, 255, 136, 0.1);
}

.slider-fill {
  position: absolute;
  height: 100%;
  background: linear-gradient(90deg, 
    transparent 0%, 
    var(--slider-color) 30%, 
    var(--slider-color) 70%, 
    transparent 100%
  );
  clip-path: polygon(0 0, calc(100% - 4px) 0, 100% 4px, 100% 100%, calc(100% - 4px) 100%, 0 100%);
  box-shadow: 
    0 0 10px var(--slider-color),
    inset 0 0 10px var(--slider-color);
  position: relative;
  overflow: hidden;
}

.fill-scanline {
  position: absolute;
  inset: 0;
  background: linear-gradient(90deg, 
    transparent 0%, 
    rgba(255, 255, 255, 0.3) 50%, 
    transparent 100%
  );
  animation: scanline 2s linear infinite;
}

.slider-wrapper.is-dragging .slider-fill {
  box-shadow: 
    0 0 20px var(--slider-color),
    0 0 30px var(--slider-color),
    inset 0 0 15px var(--slider-color);
}

.slider-thumb {
  position: absolute;
  top: 50%;
  width: 4px;
  height: 30px;
  transform: translate(-50%, -50%);
  pointer-events: none;
  z-index: 5;
}

.thumb-inner {
  position: relative;
  width: 100%;
  height: 100%;
  background: var(--slider-color);
  clip-path: polygon(0 20%, 50% 0, 100% 20%, 100% 80%, 50% 100%, 0 80%);
  box-shadow: 
    0 0 10px var(--slider-color),
    0 0 20px rgba(0, 255, 136, 0.5);
}

.slider-wrapper.is-hovering .thumb-inner {
  box-shadow: 
    0 0 15px var(--slider-color),
    0 0 30px var(--slider-color),
    0 0 40px rgba(0, 255, 136, 0.3);
}

.slider-wrapper.is-dragging .thumb-inner {
  box-shadow: 
    0 0 20px var(--slider-color),
    0 0 40px var(--slider-color),
    0 0 60px rgba(0, 255, 136, 0.5);
}

.thumb-line {
  position: absolute;
  inset: 2px;
  background: linear-gradient(180deg, 
    rgba(255, 255, 255, 0.8) 0%, 
    transparent 50%,
    rgba(255, 255, 255, 0.8) 100%
  );
}

.thumb-glow {
  position: absolute;
  inset: -5px;
  background: var(--slider-color);
  filter: blur(8px);
  opacity: 0.5;
  animation: pulse 2s ease-in-out infinite;
}

.slider-indicators {
  position: absolute;
  bottom: 0;
  left: 10px;
  right: 10px;
  height: 8px;
  display: flex;
  justify-content: space-between;
  pointer-events: none;
}

.indicator {
  font-size: 0.625rem;
  color: var(--border-color);
  line-height: 1;
  transform: translateX(-50%);
}

@keyframes scanline {
  0% {
    transform: translateX(-100%);
  }
  100% {
    transform: translateX(200%);
  }
}

@keyframes pulse {
  0%, 100% {
    opacity: 0.3;
  }
  50% {
    opacity: 0.7;
  }
}
</style>
