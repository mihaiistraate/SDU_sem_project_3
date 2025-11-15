struct coordinates{ //joystick1 - servo motor | joystick2 - DC motor
  float x1_coord = 0, y1_coord; //store the variables from the joystick
  float x2_coord = 0, y2_coord; //only the y coordinates are needed
};
coordinates set;

int led_pin = 27; //LED for power on

#define VRX1_PIN  34 //for joysticks
#define VRY1_PIN  35 //analog pins
#define VRX2_PIN  32
#define VRY2_PIN  33

int x1Value = 0, y1Value = 0; //for joysticks setup
int x2Value = 0, y2Value = 0;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);
  pinMode(VRX1_PIN, INPUT); //x1lim=[1430 3005 4090]
  pinMode(VRY1_PIN, INPUT); //y2lim=[1390 3005 4090]
  pinMode(VRX2_PIN, INPUT);
  pinMode(VRY2_PIN, INPUT);
  digitalWrite(led_pin, HIGH); //show power on
}

void loop() {
  // put your main code here, to run repeatedly:
  set.x1_coord = set.y1_coord = 0;
  float x1, y1;

  for(int i=0; i<200; ++i){
    set.x1_coord += analogRead(VRX1_PIN);
    set.y1_coord += analogRead(VRY1_PIN);
    //set.x2_coord = analogRead(VRX2_PIN);
    //set.y2_coord += analogRead(VRY2_PIN);
    delay(10);
  }
  
  x1=set.x1_coord / 2000.0;
  round(x1);
  y1=set.y1_coord / 2000.0;
  round(y1);

  Serial.print(set.x1_coord / 200);
  Serial.print(".      ");
  Serial.println(x1);
  Serial.print(set.y1_coord / 200);
  Serial.print(".      ");
  Serial.println(y1*10);
  Serial.println("");
  delay(500);
}

//x1: 865  1887 2879
//y2: 850  1861 2900
