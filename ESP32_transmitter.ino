#include <esp_now.h>
#include <WiFi.h>

esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0xa0, 0xb7, 0x65, 0xf5, 0x93, 0x90}; //Receiver MAC address
int LED_pin = 14;

#define VRX1_PIN  34 //for joysticks
#define VRY1_PIN  35 //analog pins
#define VRX2_PIN  32
#define VRY2_PIN  33

struct coordinates{
  float x1_coord; //store the variables for the joystick
  float y2_coord; //joystick1 - servo motor | joystick2 - DC motor
};
coordinates set;

void setup(){
  Serial.begin(115200);
  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, HIGH);

  WiFi.mode(WIFI_STA);
  esp_now_init();
  peerInfo.channel = 0; // Use the default Wi-Fi channel
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  esp_now_add_peer(&peerInfo);

  pinMode(VRX1_PIN, INPUT); //x1lim=[865  1887 2879] (from tests)
  pinMode(VRY1_PIN, INPUT); //y2lim=[850  1861 2900]
  pinMode(VRX2_PIN, INPUT);
  pinMode(VRY2_PIN, INPUT);
}

void loop(){
  set.x1_coord = analogRead(VRX1_PIN); //read the needed voltages/coordinates
  set.y2_coord = analogRead(VRY2_PIN);

  set.x1_coord = constrain(set.x1_coord, 865, 2879);
  set.y2_coord = constrain(set.y2_coord, 850, 2900);

  esp_now_send(broadcastAddress, (uint8_t *)&set, sizeof(set));
  delay(150);
}
