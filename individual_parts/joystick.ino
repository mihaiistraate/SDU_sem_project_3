/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-joystick
 */

#define VRX_PIN  A0 // Arduino pin connected to VRX pin | 5V operating voltage
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define S_PIN  A2 // Arduino pin connected to S pin

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int sValue = 0; // To store value of the button

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  sValue = analogRead(S_PIN);

  // print data to Serial Monitor on Arduino IDE
  Serial.print("x = ");
  Serial.print(xValue);
  Serial.print(", y = ");
  Serial.print(yValue);
  Serial.print(", button = ");
  if (sValue == 0) Serial.println("pressed");
  else Serial.println("not pressed");
  delay(2000);
}
