<template>
  <div class="data" v-if="results">
    <p>Temperature: {{ results.temperature }}°C</p>
    <p>Humidity: {{ results.humidity }}%</p>
    <div class="circle">
      <lightLvlComponent class="box" :lighLvl="results.lightLvl" />
      <waterLvlComponent class="box" :waterLvl="results.waterLvl" />
      <phLvlComponent class="box" :pHLvl="9" />
      <ppmLvlComponent class="box" :ppmValue="700" />
    </div>
    <div class="dataTime">
      <img alt="Flower" src="../assets/flower.png" />
      <p>{{ datatime }}</p>
    </div>
  </div>
</template>

<script>
import axios from "axios";
import lightLvlComponent from "./LightLvl.vue";
import waterLvlComponent from "./WaterLvl.vue";
import phLvlComponent from "./PHLvl.vue";
import ppmLvlComponent from "./PPMValue.vue";
const url = ""; //url
const config = {
  headers: {
    apikey: "", //APIKEY
    "Access-Control-Allow-Origin": "*",
    "content-type": "application/json",
    "cache-control": "no-cache",
  },
};

export default {
  components: {
    waterLvlComponent,
    phLvlComponent,
    lightLvlComponent,
    ppmLvlComponent,
  },
  data() {
    return {
      results: null,
      datatime: null,
    };
  },
  mounted() {
    axios.get(url, config).then((response) => {
      this.results = response.data.results[0].data;
      this.datatime = response.data.results[0].stream_created_at;
    });
  },
};
</script>

<style>
.h2 {
  font-weight: lighter;
}
.box {
  margin: 40px;
}

.circle {
  position: relative;
  display: flex;
  margin: 20px;
  align-items: center;
  justify-content: center;
}

.dataTime {
  margin: 30px;
  font-size: 15px;
}
</style>
