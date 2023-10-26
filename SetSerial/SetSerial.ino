// Arduino DS3232RTC Library
// https://github.com/JChristensen/DS3232RTC
// Copyright (C) 2018 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Example sketch to display the date and time from a DS3231
// or DS3232 RTC every second. Display the temperature once per
// minute. (The DS3231 does a temperature conversion once every
// 64 seconds. This is also the default for the DS3232.)
//
// Set the date and time by entering the following on the Arduino
// serial monitor:
//  year,month,day,hour,minute,second,
//
// Where
//  year can be two or four digits,
//  month is 1-12,
//  day is 1-31,
//  hour is 0-23, and
//  minute and second are 0-59.
//
// Entering the final comma delimiter (after "second") will avoid a
// one-second timeout and will allow the RTC to be set more accurately.
//
// No validity checking is done, invalid values or incomplete syntax
// in the input will result in an incorrect RTC setting.
//
// Jack Christensen 08Aug2013

#include <DS3232RTC.h>  // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>  // https://github.com/janelia-arduino/Streaming
#include <TimeLib.h>

DS3232RTC myRTC;

void setup() {
  Serial.begin(9600);

  myRTC.begin();

  Serial.println("current time");
  printDateTime(myRTC.get());

  // SET TIME
  // tmElements_t tm;
  // tm.Year = CalendarYrToTm(2023);
  // tm.Month = 4;
  // tm.Day = 17;
  // tm.Hour = 14;
  // tm.Minute = 40;
  // tm.Second = 0;

  // time_t t;
  // t = makeTime(tm);
  // myRTC.set(t);
}

void loop() {
  printDateTime(myRTC.get());

  delay(5000);
}

// print date and time to Serial
void printDateTime(time_t t) {
  printDate(t);
  Serial << ' ';
  printTime(t);
  Serial.println();
}

// print time to Serial
void printTime(time_t t) {
  printI00(hour(t), ':');
  printI00(minute(t), ':');
  printI00(second(t), ' ');
}

// print date to Serial
void printDate(time_t t) {
  printI00(day(t), 0);
  Serial << monthShortStr(month(t)) << _DEC(year(t));
}

// Print an integer in "00" format (with leading zero),
// followed by a delimiter character to Serial.
// Input value assumed to be between 0 and 99.
void printI00(int val, char delim) {
  if (val < 10) Serial << '0';
  Serial << _DEC(val);
  if (delim > 0) Serial << delim;
  return;
}

time_t compileTime() {
  const time_t FUDGE(10);  //fudge factor to allow for upload time, etc. (seconds, YMMV)
  const char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
  char compMon[4], *m;

  strncpy(compMon, compDate, 3);
  compMon[3] = '\0';
  m = strstr(months, compMon);

  tmElements_t tm;
  tm.Month = ((m - months) / 3 + 1);
  tm.Day = atoi(compDate + 4);
  tm.Year = atoi(compDate + 7) - 1970;
  tm.Hour = atoi(compTime);
  tm.Minute = atoi(compTime + 3);
  tm.Second = atoi(compTime + 6);

  time_t t = makeTime(tm);
  return t + FUDGE;  //add fudge factor to allow for compile time
}
