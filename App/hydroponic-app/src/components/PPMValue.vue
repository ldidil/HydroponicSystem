<template>
  <div>
    <div v-if="ppmValue < 300">
      <img class="water" alt="ppmValue" src="../assets/water/ppmLow.png" />
    </div>
    <div v-if="ppmValue > 300">
      <img class="water" alt="ppmValue" src="../assets/water/ppmHigh.png" />
    </div>
    <p>EC: {{ (ppmValue * 2) / 1000 }} mS/cm2</p>
  </div>
</template>

<script>
export default {
  props: ["ppmValue"],
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
