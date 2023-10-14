import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import {
  DynamoDBDocumentClient,
  ScanCommand
} from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({});

const dynamo = DynamoDBDocumentClient.from(client);

const tableName = "sensors_data";

export const handler = async (event, context) => {
  let body;
  let statusCode = 200;
  const headers = {
    "Content-Type": "text/html",
  };

  try {
    let items;
    switch (event.routeKey) {
      case "GET /sensors-data/{device_id}":
        items = await dynamo.send(
          new ScanCommand({ TableName: tableName })
        );
        const data = formatData(items.Items, event.pathParameters.device_id);
        body = `
          <!doctype html>
          <html lang="en">
            <head><title>Sensors Data</title>
            </head>
            <body>
              <div style="margin-left: 400px; margin-right: 400px;">
                ${data.map((sensor) => `
                  <h2>Sensor ${sensor.sensorAddress}</h2>
                  <canvas style="padding-bottom: 50px" id="sensor-${sensor.sensorAddress}"></canvas>
                `).join('')}
              </div>
              
              <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
              
              <script>
                ${
                  data.map((sensor) => `
                    new Chart(document.getElementById('sensor-${sensor.sensorAddress}'), {
                      type: 'line',
                      data: {
                        datasets: [{
                          yAxisID: 'Conductivity',
                          label: 'Conductivity',
                          data: [${
                            sensor.data.map((measurement) => JSON.stringify({
                                x: formatDate(measurement.receivedAt),
                                y: measurement.conductivity
                              })).join(',')
                          }],
                          borderWidth: 1
                        },
                        {
                          yAxisID: 'Temperature',
                          label: 'Temperature',
                          data: [${
                            sensor.data.map((measurement) => JSON.stringify({
                              x: formatDate(measurement.receivedAt),
                              y: measurement.temperature
                            })).join(',')
                          }],
                          borderWidth: 1
                        },
                        {
                          yAxisID: 'Water Content',
                          label: 'Water Content',
                          data: [${
                            sensor.data.map((measurement) => JSON.stringify({
                              x: formatDate(measurement.receivedAt),
                              y: measurement.waterContent
                            })).join(',')
                          }],
                          borderWidth: 1
                        }]
                      },
                      options: {
                        scales: {
                          // xAxis: {
                          //   type: 'time'
                          // }
                        }
                      }
                    });
                  `).join('')
                }
              </script>
            </body>
          </html>`;
        break;
      case "GET /sensors-data":
        items = await dynamo.send(
          new ScanCommand({ TableName: tableName })
        );
        const devices = retrieveDeviceIds(items.Items);
        body = `
          <!doctype html>
          <html lang="en">
            <head><title>Devices</title>
            </head>
            <body>
              <div style="display: flex; flex-direction: column; align-items: center; margin-top: 50px;">
                ${
                  devices.map((dev) => `<a href="https://g9p3o6ac64.execute-api.eu-central-1.amazonaws.com/sensors-data/${dev}">${dev}</a>`).join('')
                }
              </div>
            </body>
          </html>
        `;
        break;
      default:
        throw new Error(`Unsupported route: "${event.routeKey}"`);
    }
  } catch (err) {
    statusCode = 400;
    body = err.message;
  }
  
  return {
    statusCode,
    body,
    headers,
  };
};

function formatData(inputItems, deviceId) {
  const items = inputItems.filter((item) => item.device_id == deviceId);
  
  const res = [];

  items.forEach((item) => {
    Object.values(item.device_data.payload).forEach((sensorData) => {
      let sensor = res.find((sens) => sens.sensorAddress == sensorData.sensorAddress);
      if (!sensor) {
        sensor = {
          sensorAddress: sensorData.sensorAddress,
          data: []
        };
        res.push(sensor);
      }
      sensor.data.push({
        conductivity: sensorData.conductivity,
        temperature: sensorData.temperature,
        waterContent: sensorData.waterContent,
        receivedAt: item.device_data.receivedAt
      });
    });
  });
  
  return res;
}

function retrieveDeviceIds(items) {
  const devices = [];
  items.forEach((item) => {
    let device = devices.find((dev) => dev == item.device_id);
    if (!device) {
      device = item.device_id
      devices.push(device);
    }
  });
  return devices;
}

function formatDate(unformattedDate) {
  const date = new Date(unformattedDate);
  return date.getFullYear() + '-' +
    String(date.getMonth()).padStart(2, '0') + '-' +
    String(date.getDate()).padStart(2, '0') + ' ' +
    String(date.getHours()).padStart(2, '0') + ':' +
    String(date.getMinutes()).padStart(2, '0');
}
