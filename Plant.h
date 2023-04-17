const int SOIL_MOISTURE_MAX_VAL = 639;
const int SOIL_MOISTURE_MIN_VAL = 202;

class Plant {
public:
  int motorPin;
  int waterAmount;
  int soilMoisturePin;
  int soilMoistureThresholdPercentage;
  int wateringDuration;

  int Q;
  int K;

  Plant(int mp, int smp, int wa, int smtp, int q, int k) {
    motorPin = mp;
    waterAmount = wa;
    soilMoisturePin = smp;
    soilMoistureThresholdPercentage = smtp;
    Q = q;
    K = k;
    wateringDuration = waterAmount * Q + K;
  }

  void initialize() {
    digitalWrite(motorPin, HIGH);
    pinMode(motorPin, OUTPUT);
    digitalWrite(motorPin, HIGH);
  }

  void waterPlant() {
    long startTime = millis();

    while ((millis() - startTime) < wateringDuration) {
      digitalWrite(motorPin, LOW);  // TURN ON Motor Pump
    }

    digitalWrite(motorPin, HIGH);  // TURN OFF Motor Pump
  }

  bool isSoilDry() {
    int averageSoilMoisturePercentageValue = 0;
    const int numberOfMeasurements = 30;

    for (int i = 0; i < numberOfMeasurements; i++) {
      int soilMoistureValue = analogRead(soilMoisturePin);

      int soilMoisturePercentage = map(soilMoistureValue, SOIL_MOISTURE_MAX_VAL, SOIL_MOISTURE_MIN_VAL, 0, 100);
      averageSoilMoisturePercentageValue += soilMoisturePercentage;
      delay(100);
    }

    averageSoilMoisturePercentageValue /= numberOfMeasurements;

    return averageSoilMoisturePercentageValue < soilMoistureThresholdPercentage;
  }
};
