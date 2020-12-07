<template>
  <div class="container">
    <h3>Simply click a color</h3>
    <table class="table">
      <tbody>
        <tr> 
          <ColorBox color="red" daStyle="background-color: red" />
          <ColorBox color="green" daStyle="background-color: green" />
          <ColorBox color="blue" daStyle="bachkground-color: blue" />
        </tr>
      </tbody>
    </table> 
  </div> 
</template>
<script>
  import ColorBox from './ColorBox';

  export default {
    components: {
      ColorBox
    },
    name: 'ListColors'
  }
</script>
<style>
  h3 {
    margin-bottom: 5%;
  }
</style>
