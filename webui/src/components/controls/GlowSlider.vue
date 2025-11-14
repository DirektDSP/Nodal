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
  color: '#6366f1'
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
</script>

<template>
  <div class="glow-slider-container">
    <label v-if="label" class="slider-label">{{ label }}</label>
    <div 
      class="slider-wrapper"
      :class="{ 'is-dragging': isDragging, 'is-hovering': isHovering }"
      @mouseenter="handleMouseEnter"
      @mouseleave="handleMouseLeave"
      @wheel="handleWheel"
    >
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
        <div 
          class="slider-fill" 
          :style="{ 
            width: `${percentage}%`,
            '--slider-color': color
          }"
        ></div>
      </div>
      <div 
        class="slider-thumb" 
        :style="{ 
          left: `${percentage}%`,
          '--slider-color': color
        }"
      >
        <div class="thumb-glow"></div>
      </div>
    </div>
    <div class="slider-value">{{ modelValue.toFixed(1) }}</div>
  </div>
</template>

<style scoped>
.glow-slider-container {
  display: flex;
  flex-direction: column;
  gap: 0.75rem;
  padding: 0.5rem 0;
}

.slider-label {
  font-size: 0.875rem;
  font-weight: 500;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.05em;
  transition: color 0.3s ease;
}

.slider-wrapper {
  position: relative;
  height: 40px;
  display: flex;
  align-items: center;
  transition: transform 0.2s ease;
}

.slider-wrapper.is-hovering {
  transform: scale(1.02);
}

.slider-wrapper.is-hovering .slider-label {
  color: var(--text-primary);
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
  height: 6px;
  background: var(--bg-secondary);
  border-radius: 3px;
  overflow: hidden;
  box-shadow: inset 0 1px 3px rgba(0, 0, 0, 0.4);
}

.slider-fill {
  position: absolute;
  height: 100%;
  background: var(--slider-color);
  border-radius: 3px;
  transition: none;
  box-shadow: 
    0 0 10px var(--slider-color),
    0 0 20px rgba(99, 102, 241, 0.3);
}

.slider-wrapper.is-dragging .slider-fill {
  box-shadow: 
    0 0 15px var(--slider-color),
    0 0 30px rgba(99, 102, 241, 0.5),
    0 0 40px rgba(99, 102, 241, 0.3);
}

.slider-thumb {
  position: absolute;
  top: 50%;
  width: 20px;
  height: 20px;
  background: var(--slider-color);
  border-radius: 50%;
  transform: translate(-50%, -50%);
  transition: width 0.2s ease, height 0.2s ease, box-shadow 0.2s ease;
  pointer-events: none;
  box-shadow: 
    0 0 0 3px var(--bg-primary),
    0 0 10px var(--slider-color),
    0 2px 4px rgba(0, 0, 0, 0.3);
  z-index: 5;
}

.slider-wrapper.is-hovering .slider-thumb {
  width: 24px;
  height: 24px;
  box-shadow: 
    0 0 0 3px var(--bg-primary),
    0 0 15px var(--slider-color),
    0 0 25px rgba(99, 102, 241, 0.4),
    0 2px 6px rgba(0, 0, 0, 0.4);
}

.slider-wrapper.is-dragging .slider-thumb {
  width: 26px;
  height: 26px;
  box-shadow: 
    0 0 0 3px var(--bg-primary),
    0 0 20px var(--slider-color),
    0 0 35px rgba(99, 102, 241, 0.6),
    0 0 50px rgba(99, 102, 241, 0.3),
    0 2px 8px rgba(0, 0, 0, 0.5);
}

.thumb-glow {
  position: absolute;
  inset: -2px;
  border-radius: 50%;
  background: var(--slider-color);
  opacity: 0;
  filter: blur(8px);
  transition: opacity 0.3s ease;
  animation: pulse 2s ease-in-out infinite;
}

.slider-wrapper.is-hovering .thumb-glow,
.slider-wrapper.is-dragging .thumb-glow {
  opacity: 0.6;
}

.slider-value {
  font-size: 0.875rem;
  font-weight: 600;
  color: var(--text-primary);
  text-align: right;
  font-variant-numeric: tabular-nums;
  min-width: 50px;
}

@keyframes pulse {
  0%, 100% {
    transform: scale(1);
    opacity: 0.6;
  }
  50% {
    transform: scale(1.2);
    opacity: 0.8;
  }
}
</style>
