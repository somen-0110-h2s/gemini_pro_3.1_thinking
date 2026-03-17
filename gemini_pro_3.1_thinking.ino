#include <DHT.h>
#include <Adafruit_Sensor.h>

// ---------------------------------------------------------
// Configuration
// ---------------------------------------------------------
const int DHT_DATA_PIN = 4;        // Pin connected to DHT11 data line
const int STATUS_LED = 2;          // Built-in LED on ESP32 is usually GPIO 2

const unsigned long SENSOR_DELAY_MS = 5000; // Read sensor every 5 seconds
const unsigned long BLINK_DELAY_MS = 500;   // Toggle LED every 500 ms

// ---------------------------------------------------------
// Global Variables
// ---------------------------------------------------------
DHT myDHT(DHT_DATA_PIN, DHT11);

unsigned long nextSensorReadTime = 0;
unsigned long nextLedToggleTime = 0;
bool isLedOn = false;

// ---------------------------------------------------------
// Helper Functions
// ---------------------------------------------------------
void handleBlink() {
  unsigned long now = millis();
  
  if (now >= nextLedToggleTime) {
    isLedOn = !isLedOn;
    digitalWrite(STATUS_LED, isLedOn ? HIGH : LOW);
    
    // Schedule the next toggle
    nextLedToggleTime = now + BLINK_DELAY_MS;
  }
}

void handleSensor() {
  unsigned long now = millis();
  
  if (now >= nextSensorReadTime) {
    float tempC = myDHT.readTemperature();
    
    if (isnan(tempC)) {
      Serial.println("[Error] DHT11 sensor read failed.");
    } else {
      Serial.print("[Sensor] Temperature: ");
      Serial.print(tempC);
      Serial.println(" °C");
    }
    
    // Schedule the next read
    nextSensorReadTime = now + SENSOR_DELAY_MS;
  }
}

// ---------------------------------------------------------
// Main Setup & Loop
// ---------------------------------------------------------
void setup() {
  Serial.begin(115200);
  
  // Initialize LED
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);
  
  // Initialize Sensor
  myDHT.begin();
  
  Serial.println("System starting...");
  Serial.println("Reading temperature every 5 seconds...");
  Serial.println("Blinking status LED every 500 ms...");
}

void loop() {
  handleBlink();
  handleSensor();
}
