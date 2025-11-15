#include <ESP32Servo.h> //ESP32 servo library
#include <Adafruit_NeoPixel.h> //library for the rgb strip
#include <esp_now.h>
#include <WiFi.h>

const int servo_motor = 22;
const int PIN_WS2812B = 14;
const int NUM_LEDS = 17;
int j = 0, prev_y = 1860, received = 0;
Adafruit_NeoPixel strip(NUM_LEDS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

const int HIGH_SIDE1 = 17; // Connects to IO17 (P-channel)
const int LOW_SIDE1 = 26; // Connects to IO26 (N-channel, PWM capable)
const int HIGH_SIDE2 = 16; // Connects to IO16 (P-channel)
const int LOW_SIDE2 = 25; // Connects to IO25 (N-channel, PWM capable)
int motorSpeed = 0;

const int trig = 4; //ultrasonic sensor setup pins and variables
const int echo = 35;
int duration;
float distance = 10, target_distance = 15; //target_distance can be adjusted according to experiments

Servo servo; //servo motor set-up
int angle = 90;

struct coordinates{ //joystick1 - servo motor | joystick2 - DC motor
  float x1_coord = 0; //store the variables from the joystick
  float y2_coord = 0; //only the x1 & y2 coordinates are needed
};                              //x1lim=[865  1887 2879]
coordinates set;                //y2lim=[850  1861 2900]

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&set, incomingData, sizeof(set));
  received = 1;
}

//function for forward movement of the dc motor
void Forward(int speed){
  digitalWrite(HIGH_SIDE1, LOW); // Turn off P-channel high-side
  analogWrite(LOW_SIDE1, speed); // Ensure low-side on forward side is off
  digitalWrite(HIGH_SIDE2, HIGH); // Turn on P-channel high-side
  analogWrite(LOW_SIDE2, 0); // Control speed with PWM on low-side
}

//function for backward movement of the dc motor
void Reverse(int speed){
  digitalWrite(HIGH_SIDE1, HIGH); // Turn on P-channel high-side
  analogWrite(LOW_SIDE1, 0); // Control speed with PWM on low-side
  digitalWrite(HIGH_SIDE2, LOW); // Turn off P-channel high-side
  analogWrite(LOW_SIDE2, speed); // Ensure low-side on reverse side is off
}

//function to stop the dc motor
void Stop(){
  digitalWrite(HIGH_SIDE1, LOW); // Turn off P-channel high-side
  analogWrite(LOW_SIDE1, 0); // Ensure low-side is off
  digitalWrite(HIGH_SIDE2, LOW); // Turn off P-channel high-side
  analogWrite(LOW_SIDE2, 0); // Ensure low-side is off
}

void rainbow(){
  for (int i = 0; i < strip.numPixels(); ++i){ //update the rgb strip
      int pixelHue = (i * 256 / strip.numPixels() + j) & 255; //calculate hue
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue * 65536 / 256)));
  }
  strip.show(); //update the strip
  j = (j + 1) % 255;
}

void red_rgb(){
  for (int i = 0; i < strip.numPixels(); ++i)
    strip.setPixelColor(i, strip.Color(255, 0, 0)); //full red, no green or blue
  strip.show(); //apply the color changes to the strip
}

void white_rgb(){
  for (int i = 0; i < strip.numPixels(); ++i)
    strip.setPixelColor(i, strip.Color(255, 255, 255)); //full white
  strip.show(); //apply the color changes to the strip
}

void setup(){
  Serial.begin(115200);

  strip.begin(); //initialize the LED strip
  strip.show(); //clear all LEDs (turn off)

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  esp_now_init();

  pinMode(HIGH_SIDE1, OUTPUT); //dc motor pins setup
  pinMode(LOW_SIDE1, OUTPUT);
  pinMode(HIGH_SIDE2, OUTPUT);
  pinMode(LOW_SIDE2, OUTPUT);
  Stop();

  pinMode(trig, OUTPUT); //ultrasonic sensor setup
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  pinMode(echo, INPUT);

  ESP32PWM::allocateTimer(0); //servo motor setup
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	servo.setPeriodHertz(50); //standard 50 hz servo
	servo.attach(servo_motor); //attaches the servo on the right pin
  servo.write(angle); //set the servo motor position to neutral

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop(){
  if (received == 1){ //looking for the data

    //ultrasonic sensor data processing
    digitalWrite(trig, HIGH); //take a measurement with the ultrasonic sensor
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    if(duration < 38000)
      distance = duration / 59.00; //finding the distance from a potential obstacle
    
    //servo motor data processing
    if(set.x1_coord > 1900) //extend the limits of the neutral point (1887) to increase stability
      angle = map(set.x1_coord, 1900, 2879, 90, 180);  //maximum rage is 180
    else if(set.x1_coord < 1860)
      angle = map(set.x1_coord, 865, 1860, 0, 90);
    else angle = 90;
    servo.write(angle);
    
    //DC motor data processing
    if (distance <= target_distance){ //if it detects an obstacle a certain distance in front, the dc motor will have a different behaviour
      Stop();
      red_rgb();
      //delay(500);
      
      if (set.y2_coord > 1880){ //extend the limits of the neutral point (1861) to increase stability
        if(prev_y < 1840) Stop();
        Forward(40); //move forward but with minimum speed to avoid a hard collision (can be useful if there's also an obstacle in the back which cannot be detected and the car is trapped)
      }
      else if (set.y2_coord < 1840){ //move backward and allow acceleration
        if(prev_y > 1880) Stop();
        motorSpeed = 100 - map(set.y2_coord, 850, 1840, 30, 100);
        Reverse(motorSpeed);
      } else 
        Stop(); //stop the motor
      prev_y = set.y2_coord;
    }
    else{ //normal behaviour of the motor
      rainbow();

      if (set.y2_coord > 1880){ //extend the limits of the neutral point (1861) to increase stability
        if(prev_y < 1840) Stop();
        Forward(map(set.y2_coord, 1880, 2900, 30, 100)); //move forward
      }
      else if (set.y2_coord < 1840){
        if(prev_y > 1880) Stop();
        Reverse(100 - map(set.y2_coord, 850, 1840, 30, 100));
      }
      else
        Stop(); //stop the motor

      prev_y = set.y2_coord;
    }
    received = 0;
  }
  //else white_rgb();
  delay(40);
}
