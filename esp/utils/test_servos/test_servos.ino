#include <Servo.h>
#define NUM_OF_POS_SERVOS 5
#define POS_SERVO_0_PIN 2
#define POS_SERVO_1_PIN 0
#define POS_SERVO_2_PIN 4
#define POS_SERVO_3_PIN 5
#define POS_SERVO_4_PIN 16
#define CONTINUOUS_SERVO_PIN 12

Servo pos_servos[5];
Servo continuous_servo;

void setup() {
  Serial.begin(115200);
  Serial.println();

  pos_servos[0].attach(POS_SERVO_0_PIN, 1000, 2000);
  pos_servos[1].attach(POS_SERVO_1_PIN);
  pos_servos[2].attach(POS_SERVO_2_PIN);
  pos_servos[3].attach(POS_SERVO_3_PIN);
  pos_servos[4].attach(POS_SERVO_4_PIN);

  continuous_servo.attach(CONTINUOUS_SERVO_PIN);
}

void loop() {
  for (int microseconds=1000; microseconds<=2000; microseconds+=10){
    for (int i=0; i<NUM_OF_POS_SERVOS; i++){
      pos_servos[i].writeMicroseconds(microseconds);
    }
    continuous_servo.writeMicroseconds(microseconds);

    Serial.println(microseconds);
    delay(200);
  }
}
