// ---------------------------------------------------------------------------
// Sketch that does ultrasonic ping from HC-SR04 sensor and uses median of 5 samples
// to determine distance between sensor and waterline.  Distance is used to determine 
// depth of water which is read by a Raspberry Pi and logged to cloud for reporting.
// NewPing library info: https://code.google.com/p/arduino-new-ping/
// Schematics of my system: http://bantacafe.com/...
// ---------------------------------------------------------------------------

#include <NewPing.h>

#define TRIGGER_PIN  13  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     12  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
//#define buzzerPin 11     // Arduino pin ties to buzzer for high water audible alarm

// Serial variables
const int serialLength = 32; // size of the serial buffer
char serialString[serialLength];
byte serialIndex = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(115200); // Open serial monitor at 9600 baud to see ping results.
//  pinMode(buzzerPin, OUTPUT);
}

void readSerial() {
  while ((Serial.available() > 0) && (serialIndex < serialLength-1)) {
    char serialByte = Serial.read();
    if (serialByte != ';') {
      serialString[serialIndex] = serialByte;
      serialIndex++;
    }
    if (serialByte == ';' or serialIndex == (serialLength-1)) {
      parseSerial();
      serialIndex = 0;
      memset(&serialString, 0, serialLength);
    }
  }
}

void parseSerial() {
  if (strcmp(serialString, "get") == 0) {
    getDepth();
  }
}

void getDepth() {
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping_median(10); // Send ping, get ping time in microseconds (uS). # represents # of pings from which median is taken
  // Format is "channel,distance"
  Serial.print("1,");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.print("\n"); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)  
}

void loop() {
  readSerial();
//  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  //  if ((uS / US_ROUNDTRIP_CM) < 10) { // see if water is high enough to trigger an alarm
  //    digitalWrite(buzzerPin,HIGH); // trip buzzer when reading less than 10cm
  //  }
  //  else {
  //    digitalWrite(buzzerPin,LOW); // silence buzzer when reading greater than 10cm
  //  }
}

