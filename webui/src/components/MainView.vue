<script setup lang="ts">
</script>

<template>
  <main class="main-view">
    <div class="content-area">
      <div class="blank-slate">
        <h1>Audio Plugin UI</h1>
        <p>Your plugin interface goes here</p>
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
}

.content-area {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 3rem;
}

.blank-slate {
  text-align: center;
  color: var(--text-secondary);
}

.blank-slate h1 {
  font-size: 2.5rem;
  font-weight: 300;
  margin-bottom: 1rem;
  color: var(--text-primary);
}

.blank-slate p {
  font-size: 1.125rem;
  opacity: 0.7;
}
</style>
