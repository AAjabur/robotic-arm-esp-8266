#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
#include <Servo.h>

#define WIFI_SSID "CARAJAS 2.4G"
#define WIFI_PASS "FAMILIA32B"

#define WEBSOCKET_SERVER_IP_ADDRS "192.168.0.22"

WebSocketClient ws_client();

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

  while (!ws_client.isConnected()) {
    Serial.println("Trying to connect to web socket server");
		ws_client.connect(WEBSOCKET_SERVER_IP_ADDRS, "/", 9000);
  }
}

void loop() {
	if (!ws_client.isConnected()) {
		ws_client.connect(WEBSOCKET_SERVER_IP_ADDRS, "/", 9000);
    Serial.println("Still not connected to web socket server");
	} else {
    Serial.println("I am connected");
		String msg;
		if (ws_client.getMessage(msg)) {
			Serial.println(msg);
		}
	}
	delay(500);
}
