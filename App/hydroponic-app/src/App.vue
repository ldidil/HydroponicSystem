<template>
  <h1>HydroponicApp</h1>
  <dataComponent></dataComponent>
</template>

<script>
import dataComponent from "./components/DataComponent.vue";

export default {
  name: "App",
  components: {
    dataComponent,
  },
};
</script>

<style>
#app {
  font-family: Helvetica, sans-serif;
  text-align: center;
  color: #0a6d60;
  margin-top: 60px;
}

.h1 {
  font-size: 70px;
}
</style>
