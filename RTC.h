#include <RTClibExtended.h>
#include <avr/sleep.h>  // AVR library for controlling the sleep modes

RTC_DS3231 rtc;
const int wakeupPin = 2;

void initializeRTC() {
  rtc.begin();

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Clear any pending alarms
  rtc.armAlarm(1, false);
  rtc.clearAlarm(1);
  rtc.alarmInterrupt(1, false);
  rtc.armAlarm(2, false);
  rtc.clearAlarm(2);
  rtc.alarmInterrupt(2, false);

  rtc.writeSqwPinMode(DS3231_OFF); // Place SQW pin into alarm interrupt mode
}

void alarm_ISR() {
  // This runs when SQW pin is low. It will wake up the µController
  Serial.println("Wakey wakey");

  sleep_disable(); // Disable sleep mode
  detachInterrupt(digitalPinToInterrupt(wakeupPin)); // Detach the interrupt to stop it firing
}

void enterSleep() {
  // 2. Se pusta sleep mode
  sleep_enable();                       // Enabling sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Setting the sleep mode, in this case full sleep

  noInterrupts();                       // Disable interrupts
  // 3. Se postavuva pin 2 kako wakeup pin, i ceka da se razbudi
  attachInterrupt(digitalPinToInterrupt(wakeupPin), alarm_ISR, LOW);

  Serial.println("Going to sleep!");    // Print message to serial monitor
  Serial.flush();                       // Ensure all characters are sent to the serial monitor

  interrupts();                         // Allow interrupts again
  sleep_cpu();                          // Enter sleep mode

  /* The program will continue from here when it wakes */
  // Koga ke se aktivira alarmot, go izvrsuva prvo alarm_ISR(), i kodot prodolzuva od tuka
  // Disable and clear alarm
  rtc.armAlarm(1, false);
  rtc.clearAlarm(1);
  rtc.alarmInterrupt(1, false);

  Serial.println("I'm back!");          // Print message to show we're back
}
