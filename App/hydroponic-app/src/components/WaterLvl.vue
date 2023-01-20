<template>
  <div>
    <div v-if="waterLvl < 25">
      <img class="water" alt="waterLvl1" src="../assets/water/lvl1.png" />
      <p>Water level : Too low</p>
    </div>
    <div v-if="waterLvl > 25 && waterLvl < 65">
      <img class="water" alt="waterLvl2" src="../assets/water/lvl2.png" />
      <p>Water level: Optimum</p>
    </div>
    <div v-if="waterLvl > 65">
      <img class="water" alt="waterLvl1" src="../assets/water/lvl3.png" />
      <p>Water level : Too high</p>
    </div>
  </div>
</template>

<script>
export default {
  props: ["waterLvl"],
  data() {
    return {};
  },
};
</script>

<style>
.water {
  width: 90px;
  height: auto;
}
</style>
