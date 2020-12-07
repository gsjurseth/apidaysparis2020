<template>
  <td class="colorbox" v-bind:style="daStyle" @click="doit(color)" height="300"/>
</template>
<script>
  import axios from 'axios';

  export default {
    props: {
      color: {
        type: String,
        required: true
      },
      daStyle: {
        type: String,
        required: true
      }
    },
    methods: {
      doit(color) {
        let toast = this.$toasted.show(`you clicked a mighty good: ${color}`);
        toast.goAway(500);

        axios({
          method: 'post',
          url: `http://emea-poc15-test.apigee.net/v1/appcolors/${color}`,
          headers: {
            "apikey": "g7Ijk9u6y8dA4f57SggbqL8daKNdmAi5"
          }
        });
      }
    }
  }
</script>
<style>
</style>
