

function decodeUplink(input) {
  let data = [];

  for (let sensorValue = 0; sensorValue < input.bytes.length / 13; sensorValue++) {
    let sensorAddress = input.bytes[0];
    let conductivity = 0;
    let temperature = 0;
    let waterContent = 0;
  
    // ElectricalConductivity
    conductivity |= input.bytes[sensorValue + 1] << 16;
    conductivity |= input.bytes[sensorValue + 2] << 8;
    conductivity |= input.bytes[sensorValue + 3];
    conductivity = conductivity / Math.pow(10, input.bytes[sensorValue + 4]);
    
    // Temperature
    temperature |= input.bytes[sensorValue + 5] << 16;
    temperature |= input.bytes[sensorValue + 6] << 8;
    temperature |= input.bytes[sensorValue + 7];
    temperature = temperature / Math.pow(10, input.bytes[sensorValue + 8]);
    
    // VolumetricWaterContent
    waterContent |= input.bytes[sensorValue + 9] << 16;
    waterContent |= input.bytes[sensorValue + 10] << 8;
    waterContent |= input.bytes[sensorValue + 11];
    waterContent = waterContent / Math.pow(10, input.bytes[sensorValue + 12]);
    
    data.push({
      sensorAddress,
      conductivity,
      temperature,
      waterContent
    });
  }
  
  return {
    data
  };
}