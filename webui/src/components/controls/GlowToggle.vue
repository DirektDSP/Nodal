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
  colorOn: '#6366f1',
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

const sizeClasses = {
  small: 'toggle-small',
  medium: 'toggle-medium',
  large: 'toggle-large'
}
</script>

<template>
  <div class="glow-toggle-container">
    <label 
      class="toggle-wrapper"
      :class="[sizeClasses[size], { 'is-hovering': isHovering, 'is-on': modelValue }]"
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
          '--color-off': colorOff
        }"
      >
        <div class="track-glow"></div>
        <div class="toggle-thumb">
          <div class="thumb-inner"></div>
          <div class="thumb-glow"></div>
        </div>
      </div>
      <span v-if="label" class="toggle-label">{{ label }}</span>
    </label>
  </div>
</template>

<style scoped>
.glow-toggle-container {
  display: inline-flex;
  padding: 0.5rem 0;
}

.toggle-wrapper {
  display: inline-flex;
  align-items: center;
  gap: 0.75rem;
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
  border-radius: 999px;
  background: var(--color-off);
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  box-shadow: 
    inset 0 2px 4px rgba(0, 0, 0, 0.4);
  overflow: hidden;
  border: none;
  outline: none;
}

.toggle-small .toggle-track {
  width: 36px;
  height: 20px;
}

.toggle-medium .toggle-track {
  width: 48px;
  height: 26px;
}

.toggle-large .toggle-track {
  width: 60px;
  height: 32px;
}

.toggle-wrapper.is-on .toggle-track {
  background: var(--color-on);
  box-shadow: 
    inset 0 2px 4px rgba(0, 0, 0, 0.2),
    0 0 12px var(--color-on);
}

.toggle-wrapper.is-hovering.is-on .toggle-track {
  box-shadow: 
    inset 0 2px 4px rgba(0, 0, 0, 0.2),
    0 0 20px var(--color-on),
    0 0 30px rgba(99, 102, 241, 0.4);
}

.track-glow {
  position: absolute;
  inset: -4px;
  border-radius: 999px;
  background: var(--color-on);
  opacity: 0;
  filter: blur(12px);
  transition: opacity 0.3s ease;
  animation: togglePulse 2s ease-in-out infinite;
  pointer-events: none;
}

.toggle-wrapper.is-on .track-glow {
  opacity: 0.3;
}

.toggle-wrapper.is-hovering.is-on .track-glow {
  opacity: 0.5;
}

.toggle-thumb {
  position: absolute;
  top: 2px;
  left: 2px;
  background: #ffffff;
  border-radius: 50%;
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  box-shadow: 
    0 2px 4px rgba(0, 0, 0, 0.3),
    0 0 8px rgba(0, 0, 0, 0.2);
  display: flex;
  align-items: center;
  justify-content: center;
}

.toggle-small .toggle-thumb {
  width: 16px;
  height: 16px;
}

.toggle-medium .toggle-thumb {
  width: 22px;
  height: 22px;
}

.toggle-large .toggle-thumb {
  width: 28px;
  height: 28px;
}

.toggle-wrapper.is-on.toggle-small .toggle-thumb {
  transform: translateX(16px);
}

.toggle-wrapper.is-on.toggle-medium .toggle-thumb {
  transform: translateX(22px);
}

.toggle-wrapper.is-on.toggle-large .toggle-thumb {
  transform: translateX(28px);
}

.toggle-wrapper.is-hovering .toggle-thumb {
  box-shadow: 
    0 2px 6px rgba(0, 0, 0, 0.4),
    0 0 12px rgba(0, 0, 0, 0.3);
}

.toggle-wrapper.is-on .toggle-thumb {
  box-shadow: 
    0 2px 6px rgba(0, 0, 0, 0.3),
    0 0 8px var(--color-on);
}

.toggle-wrapper.is-on.is-hovering .toggle-thumb {
  box-shadow: 
    0 2px 8px rgba(0, 0, 0, 0.4),
    0 0 15px var(--color-on),
    0 0 25px rgba(99, 102, 241, 0.4);
}

.thumb-inner {
  width: 60%;
  height: 60%;
  border-radius: 50%;
  background: linear-gradient(135deg, #f0f0f0 0%, #e0e0e0 100%);
}

.thumb-glow {
  position: absolute;
  inset: -50%;
  border-radius: 50%;
  background: var(--color-on);
  opacity: 0;
  filter: blur(8px);
  transition: opacity 0.3s ease;
  pointer-events: none;
}

.toggle-wrapper.is-on .thumb-glow {
  opacity: 0.4;
  animation: thumbPulse 2s ease-in-out infinite;
}

.toggle-wrapper.is-on.is-hovering .thumb-glow {
  opacity: 0.6;
}

.toggle-label {
  font-size: 0.875rem;
  font-weight: 500;
  color: var(--text-secondary);
  transition: color 0.3s ease;
}

.toggle-wrapper.is-on .toggle-label {
  color: var(--text-primary);
}

@keyframes togglePulse {
  0%, 100% {
    opacity: 0.3;
  }
  50% {
    opacity: 0.5;
  }
}

@keyframes thumbPulse {
  0%, 100% {
    transform: scale(1);
    opacity: 0.4;
  }
  50% {
    transform: scale(1.1);
    opacity: 0.6;
  }
}
</style>
