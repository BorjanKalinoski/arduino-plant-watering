
#include <Wire.h>  // https://arduino.cc/en/Reference/Wire
#include "Plant.h"
#include "utils.h"
#include "RTC.h"
// #include <Streaming.h>  // https://github.com/janelia-arduino/Streaming

DS3232RTC::ALARM_NBR_t alarmId = DS3232RTC::ALARM_1;
DS3232RTC::ALARM_TYPES_t alarmType = DS3232RTC::ALM1_MATCH_HOURS;

constexpr uint8_t RTC_INTERRUPT_PIN = {
  2
};

// constexpr uint8_t RTC_POWER_PIN = {
//   7
// };

const int NUM_PLANTS = 1;

// Plant(motorPin, soilMoisturePin, waterAmount, soilMoistureThreshold, Q, K)
Plant sansiveria = Plant(3, A0, 400, 50, 55, 166);

Plant plants[] = {
  sansiveria,
};

void waterPlants() {
  for (int i = 0; i < NUM_PLANTS; i++) {
    if (plants[i].isSoilDry()) {
      plants[i].waterPlant();
    }
  }
}

void initializePlants() {
  for (int i = 0; i < NUM_PLANTS; i++) {
    plants[i].initialize();
  }
}

void setup() {
  // Serial.begin(9600);
  setupRTC(RTC_INTERRUPT_PIN);

  delay(500);

  resetAlarm(alarmId);

  delay(500);

  initializePlants();
  delay(500);
}


void loop() {
  setAlarm(alarmId, alarmType, 11, 00, 00);

  delay(500);

  // turnOffPin(RTC_POWER_PIN);

  goToSleep();

  delay(500);

  // turnOnPin(RTC_POWER_PIN);

  delay(500);

  resetAlarm(alarmId);

  delay(500);

  waterPlants();
}

// void printDateTime(time_t t) {
//   Serial << ((day(t) < 10) ? "0" : "") << _DEC(day(t));
//   Serial << monthShortStr(month(t)) << _DEC(year(t)) << ' ';
//   Serial << ((hour(t) < 10) ? "0" : "") << _DEC(hour(t)) << ':';
//   Serial << ((minute(t) < 10) ? "0" : "") << _DEC(minute(t)) << ':';
//   Serial << ((second(t) < 10) ? "0" : "") << _DEC(second(t));
//   Serial.println();
// }
