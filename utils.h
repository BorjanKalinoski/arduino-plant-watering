#include <avr/sleep.h>

// void turnOffPin(uint8_t pin) {
//   pinMode(pin, INPUT);
//   digitalWrite(pin, LOW);
// }

// void turnOnPin(uint8_t pin) {
//   pinMode(pin, OUTPUT);
//   digitalWrite(pin, HIGH);
// }

void goToSleep() {
  uint8_t adcsra = ADCSRA;  // save the ADC Control and Status Register A
  ADCSRA = 0;               // disable the ADC
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();  // stop interrupts to ensure the BOD timed sequence executes as required
  sleep_enable();
  sleep_bod_disable();  // disable brown-out detection while sleeping (20-25µA)
  sei();                // ensure interrupts enabled so we can wake up again
  sleep_cpu();          // go to sleep
  sleep_disable();      // wake up here
  ADCSRA = adcsra;      // restore ADCSRA
}