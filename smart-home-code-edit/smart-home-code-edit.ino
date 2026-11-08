#define BLYNK_TEMPLATE_ID "TMPL2UR2qkhHB"
#define BLYNK_TEMPLATE_NAME "SMART HOME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

#define LDR_PIN       34
#define PIR_PIN       27
#define FLAME_PIN     32
#define TRIG_PIN      5
#define ECHO_PIN      18
#define SERVO_PIN     13

#define RELAY_LIGHT   26
#define RELAY_MOTION  25
#define FAN_RELAY     14
#define LAMP1_RELAY   33
#define LAMP2_RELAY   4
#define LAMP3_RELAY   16
#define LAMP4_RELAY   17
#define PUMP_RELAY    19

Servo garageDoor;
bool garageOpen = false;
unsigned long garageTimer = 0;
unsigned long motionTimer = 0;

BlynkTimer timer;  

// ==================== Blynk Controls ====================
BLYNK_WRITE(V0) { digitalWrite(LAMP1_RELAY, param.asInt() ? LOW : HIGH); }
BLYNK_WRITE(V1) { digitalWrite(LAMP2_RELAY, param.asInt() ? LOW : HIGH); }
BLYNK_WRITE(V2) { digitalWrite(LAMP3_RELAY, param.asInt() ? LOW : HIGH); }
BLYNK_WRITE(V3) { digitalWrite(LAMP4_RELAY, param.asInt() ? LOW : HIGH); }
BLYNK_WRITE(V4) { digitalWrite(PUMP_RELAY, param.asInt() ? LOW : HIGH); }

BLYNK_WRITE(V5) {
  if (param.asInt() == 1) {
    garageDoor.write(190);
    garageOpen = true;
    garageTimer = millis();
    Serial.println("Garage Open From App");
  }
}

// ==================== HC-SR04 ====================
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}

// ==================== Tasks ====================
void checkLDR() {
  int lightValue = analogRead(LDR_PIN);
  digitalWrite(RELAY_LIGHT, (lightValue < 2000) ? HIGH : LOW);
}

void checkMotion() {
  int motion = digitalRead(PIR_PIN);
  if (motion == LOW) {
    digitalWrite(RELAY_MOTION, HIGH);
    motionTimer = millis();
    Serial.println("Motion Detected");
  }
  if (millis() - motionTimer >= 2000) {
    digitalWrite(RELAY_MOTION, LOW);
  }
}

void checkFlame() {
  int flame = digitalRead(FLAME_PIN);
  digitalWrite(FAN_RELAY, (flame == LOW) ? LOW : HIGH);
}

void checkGarage() {
  long distance = getDistance();
  if (distance > 5 && distance < 12 && !garageOpen) {
    garageDoor.write(190);
    garageOpen = true;
    garageTimer = millis();
    Serial.println("Garage Open By Sensor");
  }
  if (garageOpen && millis() - garageTimer >= 5000) {
    garageDoor.write(0);
    garageOpen = false;
    Serial.println("Garage Close");
  }
}

// ==================== Setup ====================
void setup() {
  Serial.begin(115200);

  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_MOTION, OUTPUT);
  pinMode(FAN_RELAY, OUTPUT);
  pinMode(LAMP1_RELAY, OUTPUT);
  pinMode(LAMP2_RELAY, OUTPUT);
  pinMode(LAMP3_RELAY, OUTPUT);
  pinMode(LAMP4_RELAY, OUTPUT);
  pinMode(PUMP_RELAY, OUTPUT);

  digitalWrite(RELAY_LIGHT, HIGH);
  digitalWrite(RELAY_MOTION, LOW);
  digitalWrite(FAN_RELAY, HIGH);
  digitalWrite(LAMP1_RELAY, HIGH);
  digitalWrite(LAMP2_RELAY, HIGH);
  digitalWrite(LAMP3_RELAY, HIGH);
  digitalWrite(LAMP4_RELAY, HIGH);
  digitalWrite(PUMP_RELAY, HIGH);

  pinMode(PIR_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  garageDoor.attach(SERVO_PIN);
  garageDoor.write(0);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");

  Blynk.config(BLYNK_AUTH_TOKEN);
  if (Blynk.connect()) Serial.println("Blynk Connected");

  timer.setInterval(1000L, checkLDR);
  timer.setInterval(500L, checkMotion);
  timer.setInterval(1000L, checkFlame);
  timer.setInterval(1000L, checkGarage);
}

// ==================== Loop ====================
void loop() {
  Blynk.run();
  timer.run();   
}


