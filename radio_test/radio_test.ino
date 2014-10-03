// file to read the radio controller input
/*
 * Controller output mapping (neutral positions both axes, rev for servo diretions)
 * Left stick: left/right = receiver ch4 (1370, 1930, 2480)
 * Left stick: down/up = receiver ch2 (1430, 1930, 2420)
 * Right stick: down/up = receiver ch3 (1460, 1820, 2250)
 * Right stick: left/right = receiver ch1 (2450, 1900, 1385)
 *
 * Should calibrate every time.
 */

int pin = 5;
unsigned long duration;

void setup() {
  pinMode(pin, INPUT);
  Serial.begin(9600);  
  Serial.println("Reading human input.");
  
  duration = pulseIn(pin, HIGH);
  Serial.print("In setup, duration is: ");
  Serial.println(duration);
  Serial.println("Press anything to continue to loop");
  while (!Serial.available());
  Serial.read();
}

void loop() {
  duration = pulseIn(pin,HIGH); // read the z-throttle input PWM duty-cycle
  Serial.print("duration is: ");
  Serial.println(duration);
}
