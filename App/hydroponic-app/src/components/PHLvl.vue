<template>
  <div>
    <div v-if="pHLvl < 5">
      <img class="water" alt="pHLvl" src="../assets/water/phLow.png" />
      <p>pH : Too low</p>
    </div>
    <div v-if="pHLvl >= 5 && pHLvl <= 7">
      <img class="water" alt="pHLvl" src="../assets/water/phOpt.png" />
      <p>pH: Optimum</p>
    </div>
    <div v-if="pHLvl > 7">
      <img class="water" alt="pHLvl" src="../assets/water/phHigh.png" />
      <p>pH : Too high</p>
    </div>
  </div>
</template>

<script>
export default {
  name: "pHLvl",
  props: ["pHLvl"],
  data() {
    return {};
  },
};
</script>

<style>
#pHLvl {
  margin: 20px;
}
.water {
  width: 90px;
  height: auto;
}
</style>
