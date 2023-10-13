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
    "Content-Type": "application/json",
  };

  try {
    switch (event.routeKey) {
      case "GET /sensors-data/{device_id}":
        body = await dynamo.send(
          new ScanCommand({ TableName: tableName })
        );
        const items = body.Items.filter((item) => item.device_id == event.pathParameters.device_id);
        body = formatResponse(items);
        break;
      case "GET /sensors-data":
        body = await dynamo.send(
          new ScanCommand({ TableName: tableName })
        );
        body = formatResponse(body.Items);
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

function formatResponse(items) {
  let res = [];
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
}