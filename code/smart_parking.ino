#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WIFI
const char* ssid = "sabhya";
const char* password = "sabhyagupta4xr";

// MQTT
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;

// Pins
#define SLOT1_PIN D5
#define SLOT2_PIN D6
#define GATE_SENSOR_PIN D7
#define SERVO_PIN D4

LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient espClient;
PubSubClient client(espClient);
Servo gateServo;

bool slot1, slot2, gateCar;
int freeSlots;

unsigned long gateOpenUntil = 0;
bool gateOpen = false;

void connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected");
}

void connectMQTT() {
    while (!client.connected()) {
        String clientId = "NodeMCU_SmartParking_";
        clientId += String(ESP.getChipId(), HEX);

        if (client.connect(clientId.c_str())) {
            Serial.println("MQTT Connected");
        } else {
            Serial.print("MQTT failed, rc=");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

void publishState() {
    client.publish("smartparking/slot1", slot1 ? "1" : "0", true);
    client.publish("smartparking/slot2", slot2 ? "1" : "0", true);
    client.publish("smartparking/gatecar", gateCar ? "1" : "0", true);

    String freeStr = String(freeSlots);
    client.publish("smartparking/free", freeStr.c_str(), true);

    client.publish("smartparking/gate", gateOpen ? "OPEN" : "CLOSED", true);
}

void updateLCD() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Free:");
    lcd.print(freeSlots);
    lcd.print("/2");

    lcd.setCursor(0, 1);
    lcd.print("S1:");
    lcd.print(slot1 ? "O" : "F");
    lcd.print(" S2:");
    lcd.print(slot2 ? "O" : "F");
    lcd.print(" G:");
    lcd.print(gateCar ? "1" : "0");
}

void openGateForSeconds(unsigned long ms) {
    gateServo.write(90);
    gateOpen = true;
    gateOpenUntil = millis() + ms;
}

void closeGate() {
    gateServo.write(0);
    gateOpen = false;
}

void setup() {
    Serial.begin(115200);

    pinMode(SLOT1_PIN, INPUT);
    pinMode(SLOT2_PIN, INPUT);
    pinMode(GATE_SENSOR_PIN, INPUT);

    gateServo.attach(SERVO_PIN);
    closeGate();

    Wire.begin(D2, D1);

    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Smart Parking");

    connectWiFi();

    client.setServer(mqtt_server, mqtt_port);
    connectMQTT();

    delay(1500);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
    }

    if (!client.connected()) {
        connectMQTT();
    }

    client.loop();

    // IR logic: LOW = object detected
    slot1 = (digitalRead(SLOT1_PIN) == LOW);
    slot2 = (digitalRead(SLOT2_PIN) == LOW);
    gateCar = (digitalRead(GATE_SENSOR_PIN) == LOW);

    int occupiedSlots = (slot1 ? 1 : 0) + (slot2 ? 1 : 0);
    freeSlots = 2 - occupiedSlots;

    // gate opens only if car is at gate and at least one slot is free
    if (!gateOpen && gateCar && freeSlots > 0) {
        openGateForSeconds(3000);
    }

    if (gateOpen && millis() > gateOpenUntil) {
        closeGate();
    }

    publishState();
    updateLCD();

    delay(1000);
}
