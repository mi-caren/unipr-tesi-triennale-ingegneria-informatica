import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import {
  DynamoDBDocumentClient,
  ScanCommand,
  GetCommand,
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
    switch (event.routeKey) {
      case "GET /sensors-data/{device_id}":
        body = await createHtmlPage(event.pathParameters.device_id);
        break;
      case "GET /sensors-data":
        body = await dynamo.send(
          new ScanCommand({ TableName: tableName })
        );
        body = await createHtmlPage(1);
        break;
      default:
        throw new Error(`Unsupported route: "${event.routeKey}"`);
    }
  } catch (err) {
    statusCode = 400;
    body = err.message;
  } finally {
    body = JSON.stringify(body);
  }

  return {
    statusCode,
    body,
    headers,
  };
};

async function createHtmlPage(deviceId) {
  let items = await dynamo.send(
    new ScanCommand({ TableName: tableName })
  );
  items = items.Items.filter((item) => item.device_id == deviceId);
  
  let res = [];
  let canvasElements = '';

  items.forEach((item) => {
    let device = res.find((dev) => dev.deviceId == item.device_id);
    if (!device) {
      device = {
        deviceId: item.device_id,
        data: []
      };
      
      res.push(device);
    }
    
    Object.values(item.device_data.payload).forEach((sensorData) => {
      let sensor = device.data.find((sens) => sens.sensorAddress == sensorData.sensorAddress);
      if (!sensor) {
        sensor = {
          sensorAddress: sensorData.sensorAddress,
          data: []
        };
        device.data.push(sensor);
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
  
  return `
    <!doctype html>
    <html lang="en">
      <head>
        <title>Sensors Data</title>
      </head>
      <body>
        <div style="margin-left: 150px; margin-right: 150px;">
          <canvas id="dataChart"></canvas>
        </div>
        
        <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
        
        <script>
          const ctx = document.getElementById('dataChart');
        
          new Chart(ctx, {
            type: 'line',
            data: {
              labels: ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'],
              datasets: [{
                label: '# of Votes',
                data: [12, 19, 3, 5, 2, 3],
                borderWidth: 1
              },
              {
                label: '# of Votes of another type',
                data: [4, 1, 9, 12, 3, 3],
                borderWidth: 1
              }]
            },
            options: {
              scales: {
                y: {
                  beginAtZero: true
                }
              }
            }
          });
        </script>
      </body>
    </html>
  `;
}