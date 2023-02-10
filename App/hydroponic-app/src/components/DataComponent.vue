<template>
  <div class="data" v-if="results">
    <h3>Temperature: {{ results.temperature }}°C</h3>
    <h3>Humidity: {{ results.humidity }}%</h3>
    <div class="circle">
      <lightLvlComponent class="box" :lighLvl="results.lightLvl" />
      <waterLvlComponent class="box" :waterLvl="results.waterLvl" />
      <phLvlComponent class="box" :pHLvl="results.phValue" />
      <ppmLvlComponent class="box" :ppmValue="results.tdsValue" />
    </div>
    <div class="dataTime">
      <img alt="Flower" src="../assets/flower.png" />
      <p>
        Data from :
        {{ formatDate(datatime) }}
      </p>
    </div>
  </div>
</template>

<script>
import axios from "axios";
import lightLvlComponent from "./LightLvl.vue";
import waterLvlComponent from "./WaterLvl.vue";
import phLvlComponent from "./PHLvl.vue";
import ppmLvlComponent from "./PPMValue.vue";
import dayjs from "dayjs";

const url = "http://localhost:8080/v2/devices/esp32@ldidil/streams";
const config = {
  headers: {
    apikey:
      "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImxkaWRpbCIsInJlYWRfd3JpdGUiOmZhbHNlLCJpYXQiOjE2NjgwOTg2MTN9.uUBC8Q-bKV0phQusFd9OeTAYqvvgDviBgVACKKTJ0JA",
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
  methods: {
    formatDate(dateString) {
      const date = dayjs(dateString);
      return date.format("D MMM YYYY, H:mm");
    },
  },
};
</script>

<style>
h3 {
  font-weight: normal;
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
</style>
