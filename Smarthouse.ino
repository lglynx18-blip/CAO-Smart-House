#define BLYNK_TEMPLATE_ID "TMPL6Pk9g6sZj"
#define BLYNK_TEMPLATE_NAME "CAO Proj"
#define BLYNK_AUTH_TOKEN "fvm1YWSw6LKU9UOcLa05SBVD6AyWSB9e"

#include <WiFi.h>
#include <WiFiManager.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <DHT.h>


#define DHTPIN 33
#define DHTTYPE DHT11
#define WATER_SENSOR 35
#define CLOTHES_SERVO_PIN 32

#define buzzer 12
#define triggerPIN 27
#define echoPIN 14
#define ledPIN 26
#define garageSERVO_PIN 4
#define garageTRIG_PIN 2
#define garageECHO_PIN 15


DHT dht(DHTPIN, DHTTYPE);

Servo clothesServo;
Servo s1;  // Garage servo

float temperature;
int waterValue;

bool sensorActive = false;
long duration;
int cm;
long duration2;
int distanceCm;
bool blynkButtonState = false;

BlynkTimer timer;


// V0 = Garage Door (Servo)
// V1 = Activate Ultrasonic
// V2 = Deactivate Ultrasonic
// V3 = Temperature Gauge (Display only)
// V4 = Water Sensor Gauge (Display only)

BLYNK_WRITE(V0)
{
  blynkButtonState = param.asInt();
  if (blynkButtonState) {
    s1.write(120);
    delay(3000);
    s1.write(0);
  }
}

BLYNK_WRITE(V1)
{
  int state = param.asInt();
  if (state == 1) {
    sensorActive = true;
    Serial.println("🔹 Ultrasonic sensor ACTIVATED via Blynk");
  }
  else{
    sensorActive = false;
  }
}

BLYNK_WRITE(V2)
{
  int state = param.asInt();
  if (state == 1) {
    sensorActive = false;
    noTone(buzzer);
    Serial.println("🔹 Ultrasonic sensor DEACTIVATED via Blynk");
  }
}

void garageFeature() {
  
  digitalWrite(garageTRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(garageTRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(garageTRIG_PIN, LOW);

  duration2 = pulseIn(garageECHO_PIN, HIGH);
  distanceCm = duration2 * 0.034 / 2;

  Serial.print("Garage Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  if (distanceCm > 0 && distanceCm < 60) {
    s1.write(0);
    delay(3000);
    s1.write(120);
  }
}


int getDistance() {
  digitalWrite(triggerPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPIN, LOW);

  duration = pulseIn(echoPIN, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

void smartDryerFeature() {
  temperature = dht.readTemperature();
  waterValue = analogRead(WATER_SENSOR);

  if (waterValue > 1000) {
    clothesServo.write(0); // Hide clothes
  
  } else if (temperature >= 28) {
    clothesServo.write(90); // Move to drying position
  } else {
    
    clothesServo.write(0); // Keep clothes hidden
  }

  // Send data to Blynk Gauges
  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V4, waterValue);
}


void setup() {
  Serial.begin(115200);

  pinMode(buzzer, OUTPUT);
  pinMode(triggerPIN, OUTPUT);
  pinMode(echoPIN, INPUT);
  pinMode(ledPIN, OUTPUT);
  pinMode(WATER_SENSOR, INPUT);
  pinMode(garageTRIG_PIN, OUTPUT);
  pinMode(garageECHO_PIN, INPUT);




  dht.begin();

  clothesServo.attach(CLOTHES_SERVO_PIN);
  clothesServo.write(0);

  s1.attach(garageSERVO_PIN);
  s1.write(0);

  // WiFi setup
  WiFiManager wifiManager;
  Serial.println("\nConnecting to WiFi using WiFiManager...");
  if (!wifiManager.autoConnect("ESP32_Setup", "12345678")) {
    Serial.println("Failed to connect. Restarting...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("✅ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Blynk connection
  Blynk.config(BLYNK_AUTH_TOKEN);
  if (!Blynk.connect()) {
    Serial.println("❌ Failed to connect to Blynk Cloud");
  } else {
    Serial.println("✅ Connected to Blynk Cloud");
  }

  // Timers
  timer.setInterval(2000L, garageFeature);
  timer.setInterval(3000L, smartDryerFeature);
}


void loop() {
  Blynk.run();
  timer.run();

  if (sensorActive) {
    cm = getDistance();
    Serial.print("Main Distance: ");
    Serial.print(cm);
    Serial.println(" cm");

    if (cm < 20) {
      for (int i = 0; i < 5; i++) {
        tone(buzzer, 1000, 100);
        delay(200);
      }
    } else if (cm < 100 && cm > 20) {
      digitalWrite(ledPIN, HIGH);
      delay(3000);
      digitalWrite(ledPIN, LOW);
      noTone(buzzer);
    } else {
      noTone(buzzer);
    }
  } else {
    noTone(buzzer);
  }

  delay(100);
}
