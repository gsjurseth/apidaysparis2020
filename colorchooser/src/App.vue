<template>
  <div id="app">
    I'm gonna let you choose a color from (app)
    <ListColors />
  </div>
</template>

<script>
import ListColors from './components/ListColors.vue'
export default {
  name: 'app',
  components: {
    ListColors
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>

