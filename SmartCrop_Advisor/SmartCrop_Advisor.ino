#include <SoftwareSerial.h>
#include <Wire.h>
#include "DHT.h"

// ---------------- RS485 NPK Sensor -----------------
#define RE 8
#define DE 7

SoftwareSerial mod(2, 3);  // RS485

// ---------------- DHT11 Sensor -----------------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- PH Sensor -----------------
#define PH_PIN A0
float calibration = 0.0;
float phValue = 0;

// ---------------- Soil Moisture Sensor -----------------
#define SOIL_PIN A1
int soilMoisture = 0;
String moistureStatus;

// ---------------- Crop Suggestion -----------------
String cropSuggestion;

void setup() {
  Serial.begin(9600);
  mod.begin(9600);

  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  dht.begin();
  randomSeed(analogRead(0));

  delay(500);
  Serial.println("System Initialized...");
}

void loop() {

  // -------- NPK Readings (Simulated) --------
  byte nitrogenVal    = nitrogen();
  byte phosphorousVal = phosphorous();
  byte potassiumVal   = potassium();

  // -------- DHT11 Readings --------
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // -------- pH Sensor Reading --------
  int phSensorValue = analogRead(PH_PIN);
  float voltage = phSensorValue * (5.0 / 1023.0);
  phValue = 7 + ((2.5 - voltage) / 0.18);
  phValue = phValue + calibration;

  // -------- Soil Moisture Reading --------
  soilMoisture = analogRead(SOIL_PIN);

  if (soilMoisture > 700)
    moistureStatus = "Dry";
  else if (soilMoisture > 400)
    moistureStatus = "Moderate";
  else
    moistureStatus = "Wet";

  // -------- Crop Suggestion Logic --------
  if (moistureStatus == "Wet" && temperature > 20 && phValue >= 5.5 && phValue <= 7.5) {
    cropSuggestion = "Rice";
  }
  else if (moistureStatus == "Moderate" && temperature >= 15 && temperature <= 25) {
    cropSuggestion = "Wheat";
  }
  else if (moistureStatus == "Dry" && temperature > 25) {
    cropSuggestion = "Cotton";
  }
  else if (phValue >= 6.0 && phValue <= 7.0) {
    cropSuggestion = "Vegetables";
  }
  else {
    cropSuggestion = "Soil not suitable for major crops";
  }

  // -------- Display Output --------
  Serial.println("------- Sensor Data -------");
  Serial.print("Nitrogen: "); Serial.print(nitrogenVal); Serial.println(" mg/kg");
  Serial.print("Phosphorous: "); Serial.print(phosphorousVal); Serial.println(" mg/kg");
  Serial.print("Potassium: "); Serial.print(potassiumVal); Serial.println(" mg/kg");

  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");

  Serial.print("Soil Moisture: "); Serial.print(soilMoisture);
  Serial.print(" ("); Serial.print(moistureStatus); Serial.println(")");

  Serial.print("pH Value: "); Serial.println(phValue, 2);

  Serial.print("Suggested Crop: ");
  Serial.println(cropSuggestion);

  Serial.println("----------------------------");
  delay(3000);
}

// ------------ NPK Functions (Simulation) ------------
byte nitrogen() {
  return random(10, 101);
}

byte phosphorous() {
  return random(5, 101);
}

byte potassium() {
  return random(20, 201);
}
