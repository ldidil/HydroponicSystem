<template>
  <div>
    <div v-if="lighLvl < 500">
      <img class="water" alt="lighLvl" src="../assets/water/cloud.png" />
    </div>
    <div v-if="lighLvl > 500">
      <img class="water" alt="lighLvl" src="../assets/water/sunny.png" />
    </div>
    <p>Light: {{ lighLvl }} lx</p>
  </div>
</template>

<script>
export default {
  props: ["lighLvl"],
  data() {
    return {};
  },
};
</script>

<style>
.water {
  width: 90px;
  height: auto;
  position: inherit;
}
</style>
