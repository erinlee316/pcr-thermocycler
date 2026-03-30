#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int SENSOR_PIN = 13; // Arduino pin connected to DS18B20 sensor's DQ pin

OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library

//  pcr condition 
// only change the following variables
int DENATURE_TEMP = 95;
int ANNEALING_TEMP = 55;
int EXTENSION_TEMP = 72;

int cyclec = 30;
int DENATURE_TIME = 30000; // 30 sec
int ANNEALING_TIME = 30000; // 30 sec
int EXTENSION_TIME = 60000; // 60 sec


float temp; // temperature in Celsius
//the relays connect to
int IN1 = 6;
int IN2 = 7;


#define ON   0
#define OFF  1

void setup()
{
  Serial.begin(9600); // initialize serial
  tempSensor.begin();    // initialize the sensor
  Serial.println("time,cycle,stage,phase,temperature"); // output onto serial monitor

  lcd.begin(16, 2);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  lcd.clear();

  // display on lcd
  lcd.print("Erin's Project");
  lcd.setCursor(0, 1);
  lcd.print("COVID 19 Test");

  delay(3000);
}

void loop()
{
  tempSensor.requestTemperatures(); // send the command to get temperatures
  temp = tempSensor.getTempCByIndex(0); // read temperature in Celsius

  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, HIGH);

  // Go through entire PCR cycle
  // denaturation -> annealing -> extension
  for (int i = 0; i < cyclec; i++) {

    // Denaturation 
    // heat to 95°C and hold
    while (temp < DENATURE_TEMP) {
      digitalWrite(IN2, LOW); // heater on
      tempSensor.requestTemperatures(); // send the command to get temperatures
      temp = tempSensor.getTempCByIndex(0); // read temperature in Celsius

      // display on lcd
      Serial.print(millis() / 1000.0, 1);
      Serial.print(",");
      Serial.print(i + 1);
      Serial.print(",denature,ramp,");
      Serial.println(temp, 3);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PCR ");
      lcd.print(i + 1);
      lcd.print(" Denature");
      lcd.setCursor(0, 1);
      lcd.print("Heating: ");
      lcd.print(temp);
      lcd.print((char)223);
      lcd.print("C");
      delay(500);
    }
    digitalWrite(IN2, HIGH); // heater off
    // hold at 95°C
    unsigned long denatureStart = millis();
    while (millis() - denatureStart < DENATURE_TIME) {
      tempSensor.requestTemperatures();
      float t = tempSensor.getTempCByIndex(0);
      Serial.print(millis() / 1000.0, 1); 
      Serial.print(",");
      Serial.print(i + 1); 
      Serial.print(",denature,hold,");
      Serial.println(t, 3);
      delay(500);
    }






    // Annealing 
    // cool to 55°C and hold 
    while (temp > ANNEALING_TEMP) {
      digitalWrite(IN1, LOW); // cooler on
      tempSensor.requestTemperatures();
      temp = tempSensor.getTempCByIndex(0);

      // display on lcd
      Serial.print(millis() / 1000.0, 1);
      Serial.print(",");
      Serial.print(i + 1);
      Serial.print(",anneal,ramp,");
      Serial.println(temp, 3);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PCR ");
      lcd.print(i + 1);
      lcd.print(" Anneal");
      lcd.setCursor(0, 1);
      lcd.print("Cooling: ");
      lcd.print(temp);
      lcd.print((char)223);
      lcd.print("C");
      delay(500);
    }

    digitalWrite(IN1, HIGH); // cooler off
    // hold at 55°C
    unsigned long annealStart = millis();
    while (millis() - annealStart < ANNEALING_TIME) {
      tempSensor.requestTemperatures();
      float t = tempSensor.getTempCByIndex(0);
      Serial.print(millis() / 1000.0, 1); 
      Serial.print(",");
      Serial.print(i + 1); 
      Serial.print(",anneal,hold,");
      Serial.println(t, 3);
      delay(500);
    }





    // Extension 
    // heat to 72°C and hold
    while (temp < EXTENSION_TEMP) {
      digitalWrite(IN2, LOW); // heater on
      tempSensor.requestTemperatures();
      temp = tempSensor.getTempCByIndex(0);

      // display on lcd
      Serial.print(millis() / 1000.0, 1);
      Serial.print(",");
      Serial.print(i + 1);
      Serial.print(",extension,ramp,");
      Serial.println(temp, 3);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PCR ");
      lcd.print(i + 1);
      lcd.print(" Extend");
      lcd.setCursor(0, 1);
      lcd.print("Heating: ");
      lcd.print(temp);
      lcd.print((char)223);
      lcd.print("C");
      delay(500);
    }
    digitalWrite(IN2, HIGH); // heater off

    // hold at 72°C
    unsigned long extensionStart = millis();
    while (millis() - extensionStart < EXTENSION_TIME) {
      tempSensor.requestTemperatures();
      float t = tempSensor.getTempCByIndex(0);
      Serial.print(millis() / 1000.0, 1);
      Serial.print(",");
      Serial.print(i + 1);
      Serial.print(",extension,hold,");
      Serial.println(t, 3);
      delay(500);
    }
  }






  // PCR complete
  lcd.clear();
  lcd.print("Erin's Project");
  lcd.setCursor(0, 1);
  lcd.print("PCR done");
  while (true) {} // stop
}
