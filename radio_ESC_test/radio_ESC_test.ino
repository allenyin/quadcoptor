/* ESC already configured with ESC_test, use this file
 * to relay commands from remote to ESC
 */
#include <Servo.h>

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 700
#define MOTOR_PIN 9
#define RADIO_PIN 5

#define ZMAX 2420
#define ZMIN 1430
#define ZNEU 1930

unsigned long duration;
unsigned long output;
Servo motor;

void setup() {
  Serial.begin(9600);  
  Serial.println("Setting up motor");
  motor.attach(MOTOR_PIN);
  Serial.println("Setting up radio input");
  pinMode(RADIO_PIN, INPUT);
  
  duration = pulseIn(RADIO_PIN, HIGH);
  Serial.print("In setup, duration is: ");
  Serial.println(duration);
  Serial.println("Setting low output");
  motor.writeMicroseconds(MIN_SIGNAL);
}

void loop() {
  duration = pulseIn(RADIO_PIN, HIGH);
  output = getOutput(duration);
  Serial.print("duration=");
  Serial.print(duration);
  Serial.print(" -> output=");
  Serial.println(output);
  motor.writeMicroseconds(output);
}

int getOutput(unsigned long duration) {
  if (duration <= 1930) {
    return MIN_SIGNAL;
  } else {
    return map(duration, ZNEU, ZMAX, MIN_SIGNAL, MAX_SIGNAL);
  }
}
