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
  segments?: number
}

const props = withDefaults(defineProps<Props>(), {
  min: 0,
  max: 100,
  step: 1,
  label: '',
  color: '#00ff88',
  size: 80,
  segments: 16
})

const emit = defineEmits<{
  'update:modelValue': [value: number]
}>()

const isDragging = ref(false)
const isHovering = ref(false)
const lastY = ref(0)
const lastX = ref(0)

const minAngle = -135
const maxAngle = 135
const totalRange = 270

const angle = computed(() => {
  const percentage = (props.modelValue - props.min) / (props.max - props.min)
  return minAngle + percentage * (maxAngle - minAngle)
})

const segmentArray = computed(() => {
  const gapAngle = 1 // Gap between segments in degrees
  const segmentAngle = (totalRange - (props.segments * gapAngle)) / props.segments
  
  return Array.from({ length: props.segments }, (_, i) => {
    const startAngle = minAngle + (i * (segmentAngle + gapAngle))
    const endAngle = startAngle + segmentAngle
    const midAngle = (startAngle + endAngle) / 2
    const isActive = midAngle <= angle.value
    return { 
      startAngle, 
      endAngle, 
      active: isActive 
    }
  })
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

const handleWheel = (event: WheelEvent) => {
  event.preventDefault()
  
  const direction = event.deltaY > 0 ? -1 : 1
  const newValue = props.modelValue + (direction * props.step)
  
  const clampedValue = Math.max(props.min, Math.min(props.max, newValue))
  
  emit('update:modelValue', clampedValue)
}

const updateValue = (event: MouseEvent) => {
  const deltaY = lastY.value - event.clientY
  const deltaX = event.clientX - lastX.value
  const delta = deltaY + deltaX
  
  const sensitivity = 0.5
  const change = delta * sensitivity
  
  const valueRange = props.max - props.min
  const newValue = props.modelValue + (change / 100) * valueRange
  
  const clampedValue = Math.max(props.min, Math.min(props.max, newValue))
  const steppedValue = Math.round(clampedValue / props.step) * props.step
  
  emit('update:modelValue', steppedValue)
  
  lastY.value = event.clientY
  lastX.value = event.clientX
}

// Helper function to create SVG arc path
const describeArc = (x: number, y: number, radius: number, startAngle: number, endAngle: number) => {
  const start = polarToCartesian(x, y, radius, endAngle)
  const end = polarToCartesian(x, y, radius, startAngle)
  const largeArcFlag = endAngle - startAngle <= 180 ? '0' : '1'
  
  return [
    'M', x, y,
    'L', start.x, start.y,
    'A', radius, radius, 0, largeArcFlag, 0, end.x, end.y,
    'Z'
  ].join(' ')
}

const polarToCartesian = (centerX: number, centerY: number, radius: number, angleInDegrees: number) => {
  const angleInRadians = (angleInDegrees - 90) * Math.PI / 180.0
  return {
    x: centerX + (radius * Math.cos(angleInRadians)),
    y: centerY + (radius * Math.sin(angleInRadians))
  }
}
</script>

<template>
  <div class="digital-dial-container">
    <label v-if="label" class="dial-label">{{ label }}</label>
    <div 
      class="dial-wrapper"
      :class="{ 'is-dragging': isDragging, 'is-hovering': isHovering }"
      :style="{ width: `${size}px`, height: `${size}px` }"
      @mousedown="startDrag"
      @mouseenter="isHovering = true"
      @mouseleave="isHovering = false"
      @wheel="handleWheel"
    >
      <div class="dial-frame">
        <div class="frame-corner frame-tl"></div>
        <div class="frame-corner frame-tr"></div>
        <div class="frame-corner frame-bl"></div>
        <div class="frame-corner frame-br"></div>
      </div>
      
      <svg class="dial-segments" :viewBox="`0 0 ${size - 40} ${size - 40}`" preserveAspectRatio="xMidYMid meet">
        <defs>
          <mask :id="`dialMask-${size}`">
            <rect width="100%" height="100%" fill="white"/>
            <circle :cx="(size - 40) / 2" :cy="(size - 40) / 2" :r="(size - 40) / 2 * 0.25" fill="black"/>
          </mask>
        </defs>
        <g v-for="(segment, i) in segmentArray" :key="i" :mask="`url(#dialMask-${size})`">
          <path
            :d="describeArc((size - 40) / 2, (size - 40) / 2, (size - 40) / 2 - 3, segment.startAngle, segment.endAngle)"
            :fill="segment.active ? color : 'var(--border-color)'"
            :opacity="segment.active ? 0.7 : 0.25"
            stroke="var(--bg-secondary)"
            stroke-width="0.5"
            class="segment-slice"
            :class="{ active: segment.active }"
          />
        </g>
      </svg>
      
      <div class="dial-body">
        <div class="body-grid">
          <div class="grid-line" v-for="i in 8" :key="i"></div>
        </div>
        
        <div class="dial-glow" :style="{ '--dial-color': color }"></div>
      </div>
      
      <div class="dial-markers">
        <div class="marker" style="transform: rotate(-135deg)"></div>
        <div class="marker" style="transform: rotate(-90deg)"></div>
        <div class="marker" style="transform: rotate(-45deg)"></div>
        <div class="marker" style="transform: rotate(0deg)"></div>
        <div class="marker" style="transform: rotate(45deg)"></div>
        <div class="marker" style="transform: rotate(90deg)"></div>
        <div class="marker" style="transform: rotate(135deg)"></div>
      </div>
    </div>
    <div class="dial-value">
      <span class="value-text">{{ modelValue.toFixed(1) }}</span>
    </div>
  </div>
</template>

<style scoped>
.digital-dial-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 0.75rem;
  padding: 0.5rem;
  font-family: 'Courier New', monospace;
}

.dial-label {
  font-size: 0.75rem;
  font-weight: 700;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.15em;
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

.dial-frame {
  position: absolute;
  inset: 0;
  pointer-events: none;
  z-index: 1;
}

.frame-corner {
  position: absolute;
  width: 12px;
  height: 12px;
  border: 1px solid var(--border-color);
  transition: all 0.3s ease;
}

.dial-wrapper.is-hovering .frame-corner {
  border-color: var(--dial-color);
  box-shadow: 0 0 8px var(--dial-color);
}

.frame-tl {
  top: 0;
  left: 0;
  border-right: none;
  border-bottom: none;
}

.frame-tr {
  top: 0;
  right: 0;
  border-left: none;
  border-bottom: none;
}

.frame-bl {
  bottom: 0;
  left: 0;
  border-right: none;
  border-top: none;
}

.frame-br {
  bottom: 0;
  right: 0;
  border-left: none;
  border-top: none;
}

.dial-segments {
  position: absolute;
  top: 20px;
  left: 20px;
  width: calc(100% - 40px);
  height: calc(100% - 40px);
  pointer-events: none;
  z-index: 8;
  clip-path: polygon(
    8px 0, calc(100% - 8px) 0, 100% 8px, 
    100% calc(100% - 8px), calc(100% - 8px) 100%, 
    8px 100%, 0 calc(100% - 8px), 0 8px
  );
}

.segment-slice {
  transition: all 0.1s ease;
}

.segment-slice.active {
  filter: drop-shadow(0 0 3px var(--dial-color));
}

.dial-body {
  position: absolute;
  inset: 20px;
  border-radius: 2px;
  background: linear-gradient(135deg, var(--bg-tertiary) 0%, var(--bg-secondary) 100%);
  box-shadow: 
    inset 0 0 20px rgba(0, 0, 0, 0.5),
    inset 0 2px 8px rgba(0, 0, 0, 0.4);
  display: flex;
  align-items: center;
  justify-content: center;
  overflow: hidden;
  clip-path: polygon(
    8px 0, calc(100% - 8px) 0, 100% 8px, 
    100% calc(100% - 8px), calc(100% - 8px) 100%, 
    8px 100%, 0 calc(100% - 8px), 0 8px
  );
  z-index: 2;
}

.body-grid {
  position: absolute;
  inset: 0;
  display: grid;
  grid-template-rows: repeat(8, 1fr);
  gap: 0;
  opacity: 0.1;
  z-index: 3;
}

.grid-line {
  border-top: 1px solid var(--text-primary);
}

.dial-glow {
  position: absolute;
  inset: -20%;
  background: radial-gradient(circle, var(--dial-color) 0%, transparent 70%);
  opacity: 0;
  filter: blur(20px);
  transition: opacity 0.3s ease;
  pointer-events: none;
  z-index: 7;
}

.dial-wrapper.is-hovering .dial-glow {
  opacity: 0.15;
}

.dial-wrapper.is-dragging .dial-glow {
  opacity: 0.3;
  animation: dialGlowPulse 1s ease-in-out infinite;
}

.dial-markers {
  position: absolute;
  inset: 8px;
  pointer-events: none;
  z-index: 1;
}

.marker {
  position: absolute;
  top: 0;
  left: 50%;
  width: 2px;
  height: 6px;
  background: var(--border-color);
  transform-origin: 50% calc(50% + (var(--size, 120px) / 2 - 8px));
  opacity: 0.5;
}

.dial-value {
  display: flex;
  align-items: center;
  justify-content: center;
}

.value-text {
  font-size: 0.875rem;
  font-weight: 700;
  color: v-bind(color);
  background: color-mix(in srgb, v-bind(color) 10%, transparent);
  padding: 0.25rem 0.875rem;
  border: 1px solid color-mix(in srgb, v-bind(color) 30%, transparent);
  clip-path: polygon(6px 0, calc(100% - 6px) 0, 100% 6px, 100% calc(100% - 6px), calc(100% - 6px) 100%, 6px 100%, 0 calc(100% - 6px), 0 6px);
  font-variant-numeric: tabular-nums;
  min-width: 60px;
  text-align: center;
  box-shadow: inset 0 0 10px color-mix(in srgb, v-bind(color) 10%, transparent);
  text-shadow: 0 0 8px color-mix(in srgb, v-bind(color) 50%, transparent);
}

@keyframes dialGlowPulse {
  0%, 100% {
    opacity: 0.3;
  }
  50% {
    opacity: 0.5;
  }
}
</style>
