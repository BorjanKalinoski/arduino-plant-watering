#include <RTClibExtended.h>
#include <avr/sleep.h>  // AVR library for controlling the sleep modes

RTC_DS3231 rtc;

const int wakeupPin = 2;
const int SOIL_MOISTURE_MAX_VAL = 639;
const int SOIL_MOISTURE_MIN_VAL = 202;

// SANSIVERIA VALUES

const int sansiveriaSoilMoisturePin = A0;
const int sansiveriaMotorPin = 3;
const int sansiveriaMoistureThersholdPercentage = 50;
const int sansiveriaWateringTimeMilliseconds = 15 * 1000;
// TODO see how much water is for sansiveria, threshold is around 50+

void setup() {
  digitalWrite(sansiveriaMotorPin, HIGH);
  pinMode(sansiveriaMotorPin, OUTPUT);
  pinMode(wakeupPin, INPUT_PULLUP); // Set wakeup pin as pullup
  digitalWrite(sansiveriaMotorPin, HIGH);
  Serial.begin(115200);
  //  initializeRTC();
}

void loop() {
  delay(5000);

  int averageSoilMoisturePercentageValue = 0;
  const int numberOfMeasurements = 30;
  Serial.println("Start measuring soil moisture");

  for (int i = 0; i < numberOfMeasurements; i++) {
    int soilMoistureValue = analogRead(sansiveriaSoilMoisturePin);  //put Sensor insert into soil

    int soilMoisturePercentage = map(soilMoistureValue, SOIL_MOISTURE_MAX_VAL, SOIL_MOISTURE_MIN_VAL, 0, 100);
    Serial.println(soilMoisturePercentage);
    Serial.println(soilMoistureValue);
    averageSoilMoisturePercentageValue += soilMoisturePercentage;
    delay(250);
  }

  averageSoilMoisturePercentageValue /= numberOfMeasurements;


  Serial.println("Average soil moisture percentage: ");
  Serial.println(averageSoilMoisturePercentageValue);

  Serial.println(averageSoilMoisturePercentageValue);
  Serial.println("<");



  ////  bool turnOnPump = shouldWaterPlant(sansiveriaSoilMoisturePin, sansiveriaMoistureThersholdPercentage);
  //
  ////  if (turnOnPump) {
//  waterPlant(sansiveriaMotorPin);
  ////  }
  //  //   Set alarm1 every day mm,hh,ss
  ////  rtc.setAlarm(ALM1_MATCH_HOURS, 00, 13, 00);
  //  Serial.println("KURAC!");
//  delay(15000);
  //
  //  enterSleep();  // Go to sleep
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

void alarm_ISR() {
  // This runs when SQW pin is low. It will wake up the µController
  Serial.println("Wakey wakey");

  sleep_disable(); // Disable sleep mode
  detachInterrupt(digitalPinToInterrupt(wakeupPin)); // Detach the interrupt to stop it firing
}

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

bool shouldWaterPlant(int soilMoisturePin, int soilMoistureThresholdPercentage) {
  int averageSoilMoisturePercentageValue = 0;
  const int numberOfMeasurements = 30;
  Serial.println("start measurements");

  for (int i = 0; i < numberOfMeasurements; i++) {
    int soilMoistureValue = analogRead(soilMoisturePin);  //put Sensor insert into soil

    int soilMoisturePercentage = map(soilMoistureValue, SOIL_MOISTURE_MAX_VAL, SOIL_MOISTURE_MIN_VAL, 0, 100);
    Serial.println(soilMoistureValue);
    Serial.println(soilMoisturePercentage);
    averageSoilMoisturePercentageValue += soilMoisturePercentage;
    delay(100);
  }

  averageSoilMoisturePercentageValue /= numberOfMeasurements;


  Serial.println("Average soil moisture percentage: ");
  Serial.println(averageSoilMoisturePercentageValue);

  Serial.println(averageSoilMoisturePercentageValue);
  Serial.println("<");

  Serial.println(soilMoistureThresholdPercentage);

  //  return averageSoilMoisturePercentageValue < soilMoistureThresholdPercentage;
  //  if (averageSoilMoisturePercentageValue < soilMoistureThresholdPercentage) {
  //    Serial.println("WATERING FOR");
  //    Serial.println(wateringTimeMilliseconds);
  //    Serial.println("motor pin");
  //    Serial.println(motorPin);
  //     delay(wateringTimeMilliseconds);
  //  }
  //
  //  digitalWrite(motorPin, HIGH);// TURN OFF Motor Pump
}

void waterPlant(int motorPin) {
  long startTime = millis();
  int wateringDuration = calculateWaterDur();
  Serial.println("water dur");
  Serial.println(wateringDuration);
  while (millis() - startTime < wateringDuration) {
    digitalWrite(motorPin, LOW); // TURN ON Motor Pump
  }
  digitalWrite(motorPin, HIGH); // TURN ON Motor Pump

}

int calculateWaterDur() {
  return 550 * 49 + 165;
}
