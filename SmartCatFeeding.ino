#include <Servo.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// Pin Definitions
const int TRIG_PIN = 3;
const int ECHO_PIN = 2;
const int SERVO_PIN = 9;
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin

HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

// Constants
const unsigned long FEED_INTERVAL = 20000; // 12 hours in milliseconds would be 12 * 60 * 60 * 1000
const int TARGET_WEIGHT = 65.00; // Target weight in grams
const int EMPTY_CONTAINER_DISTANCE = 10; // Distance in cm indicating empty container

// Global Variables
Servo myServo;
unsigned long previousMillis = 0;
float prevDist = 0;

void setup() {

  Serial.begin(57600);


  // Initialize the servo
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Initial position (closed)


  // Initialize the ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 4.20; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}

void loop() {


 //----------- weight sensor -------------- //

  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;



  unsigned long currentMillis = millis();
  if (prevDist != getDistance() and prevDist > EMPTY_CONTAINER_DISTANCE) currentMillis = FEED_INTERVAL + previousMillis;
  
  // Check if it's time to feed
  if (newDataReady) {
    if (currentMillis - previousMillis >= FEED_INTERVAL) {
      previousMillis = currentMillis;

      // Get the weight of the food in the bowl
      float foodWeight = LoadCell.getData();

      // Check if the food container is empty
      if (getDistance() > EMPTY_CONTAINER_DISTANCE) { // Food container is empty
        Serial.println("Food container is empty. Please refill.");
      }
      else if( foodWeight >= TARGET_WEIGHT) { // Bowl is full
        Serial.println("Bowl is still full from last feed.");
      }
      else { // Bowl is empty and there is food in the container
        myServo.write(90); // Open the food container

        while (true) {
          LoadCell.update();
          foodWeight = LoadCell.getData();
          Serial.println(foodWeight);

          // Check if the target weight has been reached
          if (foodWeight >= TARGET_WEIGHT) {
            Serial.println("Closing. Food Dispensed");
            break;
          }

          // Check if container has been emptied while food is dispensing
          if (getDistance() > EMPTY_CONTAINER_DISTANCE) { 
            Serial.println("Food container is empty. Please refill.");
            break; 
          }
        }
        myServo.write(0); // Close the food container
        newDataReady = 0;
      }
    }
  }
}


// Function to get distance from ultrasonic sensor
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; // Convert to cm
  
  return distance;
}
