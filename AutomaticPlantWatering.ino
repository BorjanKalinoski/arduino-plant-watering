#include "RTC.h"
#include "Plant.h"

Plant plants[] = {
  // Plant(motorPin, soilMoisturePin, waterAmount, soilMoistureThreshold, Q, K)
  Plant(3, A0, 300 , 50, 49, 165), // Sansiveria
};

const int NUM_PLANTS = 1;


void setup() {
  for (int i = 0; i < NUM_PLANTS; i++) {
    plants[i].initialize();
  }

  pinMode(wakeupPin, INPUT_PULLUP); // Set wakeup pin as pullup

  Serial.begin(115200);
  initializeRTC();
}


void loop() {
  delay(5000);

  for (int i = 0; i < NUM_PLANTS; i++) {
    if (plants[i].isSoilDry()) {
      plants[i].waterPlant();
    }
  }

  // Set alarm1 every day @ mm, hh, ss
  rtc.setAlarm(ALM1_MATCH_HOURS, 00, 13, 00);
  enterSleep();  // Go to sleep
}
