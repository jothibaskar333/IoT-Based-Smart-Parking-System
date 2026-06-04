IoT-Based Smart Parking System

This project implements a smart parking management system using the NodeMCU ESP8266 microcontroller, IR sensors, MQTT communication, and a servo-controlled gate. The system monitors parking slot occupancy in real time, automatically controls vehicle entry based on slot availability, and provides both local and remote parking status updates.

Features
Real-time parking slot occupancy detection using IR sensors
Automated gate control using servo motor actuation
MQTT-based wireless communication for remote monitoring
Live parking status display through an I2C LCD
Mobile dashboard integration using IoT MQTT Panel
Layered IoT architecture (Perception, Processing, Communication, and Application layers)
Technologies Used
NodeMCU ESP8266
Arduino IDE
Embedded C/C++
MQTT Protocol
ESP8266WiFi Library
PubSubClient Library
IR Sensors
Servo Motor
I2C LCD Display
IoT MQTT Panel
System Architecture

The system follows a layered IoT architecture:

Perception Layer – IR sensors detect vehicle presence and parking slot occupancy.
Processing Layer – NodeMCU ESP8266 processes sensor data and executes gate-control logic.
Communication Layer – MQTT protocol transmits parking status over Wi-Fi.
Application Layer – Mobile dashboard displays real-time parking information.
Key Learning Outcomes
Embedded systems programming
IoT architecture design
MQTT publish/subscribe communication
Sensor interfacing and actuator control
Real-time monitoring systems
Wireless IoT communication
