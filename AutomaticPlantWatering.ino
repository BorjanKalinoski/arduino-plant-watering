const int drySoilMoistureValue = 639;
const int wetSoilMoistureValue = 253;
const int motorPin = 3;
const int waitTime = 5000;

const int soilMoisturePin = A0;

int soilMoistureValue = 0;
int soilMoisturePercentage = 0;

void setup() {
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}

void loop() {
  soilMoistureValue = analogRead(soilMoisturePin);  //put Sensor insert into soil

  Serial.println("Soil moisture value:");
  Serial.println(soilMoistureValue);

  soilMoisturePercentage = map(soilMoistureValue, drySoilMoistureValue, wetSoilMoistureValue, 0, 100);

  Serial.println("Soil moisture percentage: ");
  Serial.println(soilMoisturePercentage);

  if (soilMoisturePercentage < SANSIVERIA_MOISTURE_THRESHOLD) {
    digitalWrite(motorPin, LOW); // TURN ON Motor Pump
    delay(SANSIVERIA_WATER_TIME);
  }

  digitalWrite(motorPin, HIGH);// TURN OFF Motor Pump
  delay(waitTime);
}

const int SANSIVERIA_MOISTURE_THRESHOLD = 10;
const int SANSIVERIA_WATER_TIME = 10000;
