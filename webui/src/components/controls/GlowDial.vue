<script setup lang="ts">
import { ref, computed } from 'vue'

interface Props {
  modelValue: number
  min?: number
  max?: number
  step?: number
  label?: string
  color?: string
  size?: number
  startPosition?: 'bottom' | 'left' | 'top'
}

const props = withDefaults(defineProps<Props>(), {
  min: 0,
  max: 100,
  step: 1,
  label: '',
  color: '#6366f1',
  size: 80,
  startPosition: 'bottom'
})

const emit = defineEmits<{
  'update:modelValue': [value: number]
}>()

const isDragging = ref(false)
const isHovering = ref(false)
const dialRef = ref<HTMLDivElement | null>(null)
const lastY = ref(0)
const lastX = ref(0)

// Calculate angles based on start position
const getAngles = () => {
  switch (props.startPosition) {
    case 'left':
      return { minAngle: -180, maxAngle: 0, totalRange: 180 }
    case 'top':
      return { minAngle: -90, maxAngle: 90, totalRange: 180 }
    case 'bottom':
    default:
      return { minAngle: -135, maxAngle: 135, totalRange: 270 }
  }
}

const { minAngle, maxAngle, totalRange } = getAngles()

const angle = computed(() => {
  const percentage = (props.modelValue - props.min) / (props.max - props.min)
  return minAngle + percentage * (maxAngle - minAngle)
})

const normalizedValue = computed(() => {
  return ((props.modelValue - props.min) / (props.max - props.min)) * 100
})

const arcLength = computed(() => {
  const radius = (props.size / 2) - 8
  const circumference = 2 * Math.PI * radius
  return (normalizedValue.value / 100) * (totalRange / 360) * circumference
})

const startDrag = (event: MouseEvent) => {
  isDragging.value = true
  lastY.value = event.clientY
  lastX.value = event.clientX
  
  const handleMove = (e: MouseEvent) => {
    if (isDragging.value) {
      updateValue(e)
    }
  }
  
  const handleUp = () => {
    isDragging.value = false
    document.removeEventListener('mousemove', handleMove)
    document.removeEventListener('mouseup', handleUp)
  }
  
  document.addEventListener('mousemove', handleMove)
  document.addEventListener('mouseup', handleUp)
}

const updateValue = (event: MouseEvent) => {
  const deltaY = lastY.value - event.clientY // Inverted: up = positive
  const deltaX = event.clientX - lastX.value // Right = positive
  
  // Combine both axes for intuitive control
  const delta = deltaY + deltaX
  
  // Sensitivity adjustment
  const sensitivity = 0.5
  const change = delta * sensitivity
  
  const valueRange = props.max - props.min
  const newValue = props.modelValue + (change / 100) * valueRange
  
  // Clamp to min/max
  const clampedValue = Math.max(props.min, Math.min(props.max, newValue))
  const steppedValue = Math.round(clampedValue / props.step) * props.step
  
  emit('update:modelValue', steppedValue)
  
  lastY.value = event.clientY
  lastX.value = event.clientX
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
  <div class="glow-dial-container">
    <label v-if="label" class="dial-label">{{ label }}</label>
    <div 
      ref="dialRef"
      class="dial-wrapper"
      :class="{ 'is-dragging': isDragging, 'is-hovering': isHovering }"
      :style="{ width: `${size}px`, height: `${size}px` }"
      @mousedown="startDrag"
      @mouseenter="isHovering = true"
      @mouseleave="isHovering = false"
      @wheel="handleWheel"
    >
      <svg class="dial-arc" :viewBox="`0 0 ${size} ${size}`">
        <defs>
          <linearGradient id="arcGradient" x1="0%" y1="0%" x2="100%" y2="100%">
            <stop offset="0%" :style="{ stopColor: color, stopOpacity: 0.3 }" />
            <stop offset="100%" :style="{ stopColor: color, stopOpacity: 1 }" />
          </linearGradient>
          <filter id="glow">
            <feGaussianBlur stdDeviation="2" result="coloredBlur"/>
            <feMerge>
              <feMergeNode in="coloredBlur"/>
              <feMergeNode in="SourceGraphic"/>
            </feMerge>
          </filter>
        </defs>
        
        <!-- Background arc -->
        <circle
          :cx="size / 2"
          :cy="size / 2"
          :r="(size / 2) - 8"
          fill="none"
          stroke="var(--bg-secondary)"
          :stroke-width="6"
          stroke-linecap="round"
          :stroke-dasharray="`${((size / 2) - 8) * 2 * Math.PI * (totalRange / 360)}, ${1000}`"
          :transform="`rotate(${minAngle} ${size / 2} ${size / 2})`"
          opacity="0.3"
        />
        
        <!-- Value arc -->
        <circle
          :cx="size / 2"
          :cy="size / 2"
          :r="(size / 2) - 8"
          fill="none"
          :stroke="color"
          :stroke-width="6"
          stroke-linecap="round"
          :stroke-dasharray="`${arcLength}, ${1000}`"
          :transform="`rotate(${minAngle} ${size / 2} ${size / 2})`"
          filter="url(#glow)"
          class="value-arc"
        />
      </svg>
      
      <div class="dial-body">
        <div 
          class="dial-indicator" 
          :style="{ 
            transform: `rotate(${angle}deg)`,
            '--dial-color': color
          }"
        >
          <div class="indicator-line"></div>
          <div class="indicator-dot"></div>
        </div>
        <div class="dial-center"></div>
        <div class="dial-glow" :style="{ '--dial-color': color }"></div>
      </div>
    </div>
    <div class="dial-value">{{ modelValue.toFixed(1) }}</div>
  </div>
</template>

<style scoped>
.glow-dial-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 0.75rem;
  padding: 0.5rem;
}

.dial-label {
  font-size: 0.875rem;
  font-weight: 500;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.05em;
  transition: color 0.3s ease;
}

.dial-wrapper {
  position: relative;
  cursor: pointer;
  transition: transform 0.2s ease;
  user-select: none;
}

.dial-wrapper.is-hovering {
  transform: scale(1.05);
}

.dial-wrapper.is-hovering .dial-label {
  color: var(--text-primary);
}

.dial-arc {
  position: absolute;
  inset: 0;
  pointer-events: none;
  transition: filter 0.3s ease;
}

.value-arc {
  transition: none;
}

.dial-wrapper.is-dragging .value-arc {
  filter: url(#glow) drop-shadow(0 0 8px var(--dial-color));
}

.dial-body {
  position: absolute;
  inset: 12px;
  border-radius: 50%;
  background: linear-gradient(135deg, var(--bg-secondary) 0%, var(--bg-tertiary) 100%);
  box-shadow: 
    inset 0 2px 8px rgba(0, 0, 0, 0.4),
    0 2px 8px rgba(0, 0, 0, 0.3);
  display: flex;
  align-items: center;
  justify-content: center;
  overflow: hidden;
}

.dial-indicator {
  position: absolute;
  width: 100%;
  height: 100%;
  transition: none;
  pointer-events: none;
}

.indicator-line {
  position: absolute;
  top: 15%;
  left: 50%;
  width: 3px;
  height: 35%;
  background: linear-gradient(to bottom, var(--dial-color) 0%, transparent 100%);
  transform: translateX(-50%);
  border-radius: 2px;
  box-shadow: 0 0 8px var(--dial-color);
}

.indicator-dot {
  position: absolute;
  top: 12%;
  left: 50%;
  width: 6px;
  height: 6px;
  background: var(--dial-color);
  border-radius: 50%;
  transform: translateX(-50%);
  box-shadow: 
    0 0 8px var(--dial-color),
    0 0 12px rgba(99, 102, 241, 0.5);
}

.dial-center {
  position: absolute;
  width: 30%;
  height: 30%;
  background: radial-gradient(circle, var(--bg-primary) 0%, var(--bg-secondary) 100%);
  border-radius: 50%;
  box-shadow: 
    0 2px 4px rgba(0, 0, 0, 0.3),
    inset 0 1px 2px rgba(255, 255, 255, 0.1);
  z-index: 2;
}

.dial-glow {
  position: absolute;
  inset: -10%;
  border-radius: 50%;
  background: radial-gradient(circle, var(--dial-color) 0%, transparent 70%);
  opacity: 0;
  filter: blur(15px);
  transition: opacity 0.3s ease;
  animation: dialPulse 3s ease-in-out infinite;
  pointer-events: none;
}

.dial-wrapper.is-hovering .dial-glow {
  opacity: 0.2;
}

.dial-wrapper.is-dragging .dial-glow {
  opacity: 0.4;
  animation: dialPulse 1s ease-in-out infinite;
}

.dial-value {
  font-size: 0.875rem;
  font-weight: 600;
  color: var(--text-primary);
  font-variant-numeric: tabular-nums;
  min-width: 50px;
  text-align: center;
}

@keyframes dialPulse {
  0%, 100% {
    transform: scale(0.95);
    opacity: 0.2;
  }
  50% {
    transform: scale(1.05);
    opacity: 0.3;
  }
}
</style>
