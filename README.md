# CAO-Smart-House
This project integrates a Smart Clothes Dryer and an Automated Garage System using an ESP32, Blynk IoT, and various sensors.


BLYNK-Based Smart Clothes Dryer & Garage System (ESP32)

This project integrates a Smart Clothes Dryer and an Automated Garage System using an ESP32, Blynk IoT, and various sensors.

The system connects to WiFi via WiFiManager and communicates with the Blynk mobile app, allowing remote monitoring and control.

🔹 Main Features

Smart Clothes Dryer

Uses a DHT11 sensor to measure temperature and a water sensor to detect rain or humidity.

Automatically moves clothes in or out using a servo motor:

Clothes retracted when it’s raining (waterValue > 1000)

Clothes extended when the temperature is hot (≥ 28°C)

Sends live temperature and water sensor data to Blynk gauges (V3, V4).

Automated Garage Door

Controlled manually via Blynk button (V0) or automatically using an ultrasonic sensor to detect nearby vehicles.

Opens or closes the garage servo motor based on detected distance (< 60 cm).

Ultrasonic Security System

Activated or deactivated through Blynk buttons (V1, V2).

Measures distance and triggers a buzzer and LED warning when an object or vehicle is too close (< 20 cm).

🔹 Connectivity & Control

WiFiManager provides easy WiFi setup via an access point (“ESP32_Setup”).

Blynk Cloud allows remote control, live data display, and feature toggling.

🔹 Hardware Components

ESP32

DHT11 (Temperature & Humidity)

Water Sensor

Two Servo Motors (Clothes & Garage)

Two Ultrasonic Sensors

Buzzer and LED Indicators
