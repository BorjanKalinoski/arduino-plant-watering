#include <DS3232RTC.h>  // https://github.com/JChristensen/DS3232RTC

DS3232RTC myRTC;

void alarm_ISR() {}

void resetAlarm(DS3232RTC::ALARM_NBR_t alarm) {
  myRTC.alarm(alarm);  // clear alarm flag
  myRTC.alarmInterrupt(alarm, false);
}

void setAlarm(DS3232RTC::ALARM_NBR_t alarm, DS3232RTC::ALARM_TYPES_t alarmType, uint8_t hours, uint8_t minutes, uint8_t seconds) {
  myRTC.setAlarm(alarmType, seconds, minutes, hours, 1);
  myRTC.alarmInterrupt(alarm, true);
}

void setupRTC(uint8_t rtcInterruptPin) {
  myRTC.begin();

  pinMode(rtcInterruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rtcInterruptPin), alarm_ISR, FALLING);

  myRTC.setBBSQW(true);  // enable battery backed alarm
}
