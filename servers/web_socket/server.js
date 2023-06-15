const WebSocket = require('ws');
const get_local_ip_address = require("./utils/ip_utils.js");

const wss = new WebSocket.Server({ port: 9000 });
const ESP_SECRET_ID = "meu_segredinho";
let esp_client = null;

console.log(wss.address())
const ip_address = get_local_ip_address();
if (ip_address) {
  console.log(`WebSocket server is running at ${ip_address}:9000`);
} else {
  console.log('Unable to determine the local IP address');
}

wss.on('connection', (ws, req) => {
  console.log("Received a New Connection");
  if (req.headers.authorization==ESP_SECRET_ID){
    esp_client = ws;
    console.log("Esp connected");

    return
  }

  ws.on('message', (message) => {
    parsed_angles = parse_angles_message(message);

    if (esp_client != null){
      esp_client.send(message)
    }

  });

  ws.on('close', () => {
    console.log('Client disconnected');
    client = null;
  });
});

function parse_angles_message(message){
  return {
    "servo_0": message.readUInt16LE(0),
    "servo_1": message.readUInt16LE(2),
    "servo_2": message.readUInt16LE(4),
    "servo_3": message.readUInt16LE(6),
    "servo_4": message.readUInt16LE(8),
    "servo_5": message.readUInt16LE(10),
  }
}
