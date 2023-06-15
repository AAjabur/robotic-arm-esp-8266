#include <WiFi.h>
#include <ESP32Servo.h>
#include <WebSocketClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define SERVO_GPIO 2
#define WEBSOCKET_SERVER_IP_ADDRS "192.168.43.214"

#define CONTINUOUS_SERVO_MIN_PERIOD_MS 1000
#define CONTINUOUS_SERVO_MAX_PERIOD_MS 2000
#define CONTINUOUS_SERVO_ZERO_PERIOD_MS 1500
#define PI 3.14159
#define SERVO_0_PIN 13
#define SERVO_1_PIN 14
#define SERVO_2_PIN 2
#define SERVO_3_PIN 12
#define SERVO_4_PIN 4
#define COUNTINUOUS_SERVO_PIN 15

const char* wifi_ssid = "ASUS_X01BDA";
const char* wifi_password = "12345678";

WebSocketClient ws_client;
Adafruit_MPU6050 mpu;

Servo servos[5];
Servo continuous_servo;

float goal_z_angle = 0;
float z_relative_angle = 0;

void proccess_internet_reconnections(void *parameters) {
  while(1) {
    if (!ws_client.isConnected()) {
      ws_client.connect(WEBSOCKET_SERVER_IP_ADDRS, "/", 9000);
      Serial.println("Not connected to websocket server");
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Not connected to wifi");
      WiFi.disconnect();
      WiFi.reconnect();
    }
    vTaskDelay( 20000 / portTICK_PERIOD_MS);
  }
}

void update_relative_z_angle(void *parameters) {
  while(1) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    z_relative_angle += (g.gyro.z + 0.027)*0.02;

    vTaskDelay( 20 / portTICK_PERIOD_MS);
  }
}

void receive_angle_messages(void *parameters) {
  while (1){
    unsigned char msg_buf[13];
    String msg_str;
    uint16_t servo_angles[6] = {1500, 1500, 1500, 1500, 1500, 1500};
    if (ws_client.getMessage(msg_str)) {
      Serial.println("Received a Message");

      while(ws_client.getMessage(msg_str)); // get only the last msg received

      msg_str.getBytes(msg_buf, 13);

      for (int i = 0; i != 6; i++){
        servo_angles[i] = (uint16_t)((msg_buf[2*i+1] << 8) | msg_buf[2*i]);
      }

      for (int i=0; i != 5; i++){
        servos[i].writeMicroseconds(servo_angles[i]);
      }
    }

    goal_z_angle = servo_angles[5] * 2*PI / 1000;
    vTaskDelay( 100 / portTICK_PERIOD_MS);
  }
}

void control_z_servo(void *parameters) {
  while (1) {
    float error_angle = goal_z_angle - z_relative_angle;

    // continuous_servo.writeMicroseconds(CONTINUOUS_SERVO_ZERO_PERIOD_MS + error_angle*5);
    continuous_servo.writeMicroseconds(1500);
    vTaskDelay( 100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial){delay(100);}
  
  Serial.print("\nConnecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  ws_client.setAuthorizationHeader("meu_segredinho");
  ws_client.connect(WEBSOCKET_SERVER_IP_ADDRS, "/", 9000);

  int servo_pins[5] = {SERVO_0_PIN, SERVO_1_PIN, SERVO_2_PIN, SERVO_3_PIN, SERVO_4_PIN};
  for (int i; i!=5; i++){
    servos[i].attach(servo_pins[i], 200, 2600);
  }

  continuous_servo.attach(COUNTINUOUS_SERVO_PIN, CONTINUOUS_SERVO_MIN_PERIOD_MS, CONTINUOUS_SERVO_MAX_PERIOD_MS);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  xTaskCreate(
    proccess_internet_reconnections,
    "proccess_internet_reconnections",
    5000,
    NULL,
    0,
    NULL
  );

  xTaskCreate(
    receive_angle_messages,
    "receive_angle_messages",
    5000,
    NULL,
    0,
    NULL
  );

  xTaskCreate(
    update_relative_z_angle,
    "update_relative_z_angle",
    5000,
    NULL,
    0,
    NULL
  );

  xTaskCreate(
    control_z_servo,
    "control_z_servo",
    5000,
    NULL,
    0,
    NULL
  );
}

void loop() {

}
