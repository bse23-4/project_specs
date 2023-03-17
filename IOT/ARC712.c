/*
This code reads the analog value from the ACS712 sensor and converts it to a power value. It then calculates the energy consumed since the last reading and adds it to a running total of energy. Every hour, it calculates the total energy consumption in units (kWh) and prints it to the serial monitor. Note that you'll need to configure the voltage divider ratio and the number of pulses per kWh based on your circuit. You'll also need to connect a pulse output from your energy meter to a digital input pin on your microcontroller to count the energy consumption.
*/
// Include the Wire library for I2C communication
#include <Wire.h>

// Define the address of the ACS712T 30A sensor
#define ACS712_ADDRESS 0x4A

// Define the analog input pin for the ACS712 sensor
#define ACS712_ANALOG_PIN A0

// Define the offset value of the ACS712 sensor
#define ACS712_OFFSET 2500

// Define the voltage reference for the ADC
#define ADC_REF_VOLTAGE 5

// Define the voltage divider ratio for voltage measurement
#define VOLTAGE_DIVIDER_RATIO 2

// Define the maximum current the sensor can measure
#define MAX_CURRENT 30.0

// Define the number of pulses per kWh
#define PULSES_PER_KWH 1000

// Define the time interval between energy readings
#define ENERGY_READ_INTERVAL 3600000 // 1 hour in milliseconds

// Initialize variables for energy monitoring
unsigned long lastEnergyReadTime = 0;
float currentEnergy = 0;
float lastCurrent = 0;

void setup() {
  // Start the I2C communication
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // Read the raw analog value from the ACS712 sensor
  int rawValue = analogRead(ACS712_ANALOG_PIN);

  // Calculate the voltage from the raw analog value
  float voltage = (float)rawValue * ADC_REF_VOLTAGE / 1023.0 / VOLTAGE_DIVIDER_RATIO;

  // Calculate the current from the voltage and the offset value
  float current = (voltage - ACS712_OFFSET) / (MAX_CURRENT / 1024.0);

  // Calculate the power from the current and the voltage
  float power = voltage * current;

  // Print the current power value to the serial monitor
  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" W");

  // Calculate the energy consumed since the last reading
  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - lastEnergyReadTime;
  float energy = lastCurrent * deltaTime / 3600000.0; // Convert milliseconds to hours

  // Add the energy to the current energy total
  currentEnergy += energy;

  // Check if it's time to read the energy consumption
  if (currentTime - lastEnergyReadTime >= ENERGY_READ_INTERVAL) {
    // Calculate the energy consumption in units (kWh)
    float energyUnits = currentEnergy / 1000.0;

    // Print the energy consumption to the serial monitor
    Serial.print("Energy Consumption: ");
    Serial.print(energyUnits);
    Serial.println(" kWh");

    // Reset the current energy total and update the last energy read time
    currentEnergy = 0;
    lastEnergyReadTime = currentTime;
  }

  // Update the last current value
  lastCurrent = current;

  // Wait for a short period before reading the sensor again
  delay(1000);
}
