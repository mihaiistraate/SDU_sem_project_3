int trig = 22; //port D9
int echo = 23; //port D8
int duration;
float distance;

void setup(){
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  pinMode(echo, INPUT);
  delay(6000);
}

void loop(){
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  if(duration>=38000){
    Serial.println("Out range");
  }
  else{
    distance = duration/59.00;
    Serial.print(distance);
    Serial.println("cm");
  }
  delay(1500);
}
