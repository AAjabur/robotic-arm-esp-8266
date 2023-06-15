#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
#include <stdint.h>

#define WIFI_SSID "CARAJAS 2.4G"
#define WIFI_PASS "FAMILIA32B"

#define WEBSOCKET_SERVER_IP_ADDRS "192.168.0.22"

struct ServoConfig {
  Servo servo;
  int pin;
  int min_period_ms;
  int max_period_ms;
  int zero_period_ms;
};

WebSocketClient ws_client;

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  ws_client.setAuthorizationHeader("meu_segredinho");
}

void loop() {
	while(!ws_client.isConnected()) {
		ws_client.connect(WEBSOCKET_SERVER_IP_ADDRS, "/", 9000);
    Serial.println("Still not connected to web socket server");
    delay(500);
	}
  Serial.println("I am connected");

  unsigned char msg_buf[13];
  String msg_str;
  if (ws_client.getMessage(msg_str)) {
    Serial.println("Received a Message");
    msg_str.getBytes(msg_buf, 13);

    uint16_t servo_angles[6];
    for (int i = 0; i != 6; i++){
      servo_angles[i] = (uint16_t)((msg_buf[2*i+1] << 8) | msg_buf[2*i]);

      Serial.print("Servo ");
      Serial.print(i);
      Serial.print("\n\t Period: ");
      Serial.println(servo_angles[i]);
    }
  }
}
