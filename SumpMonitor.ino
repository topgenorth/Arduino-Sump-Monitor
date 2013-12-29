#include <LiquidCrystal.h>
#include <CapacitiveSensor.h>
#include <SPI.h>
#include <Ethernet.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xC1, 0xFD, 0xED };

EthernetClient client;

// ThingSpeak Settings
byte server[]  = { 184, 106, 153, 149 }; // IP Address for the ThingSpeak API
String writeAPIKey = "YOUR ThiingsSpeak API KEY HERE";    // Write API Key for a ThingSpeak Channel
const int updateInterval = 15000;        // Time interval in milliseconds to update ThingSpeak   

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int resetCounter = 0;

#define BUTTON_PIN A5    // The number of the push-button pin.
#define LCD_LIGHT_PIN A4 // The number of the pin where anode of the display backlight is.
#define LCD_LIGHT_ON_TIME 60000 // How long (in milliseconds) should lcd light stay on?
#define LCDlength 16.0
//#define BUZZER_PIN A?; //Pin buzzer

//unsigned char b;
//unsigned int peace; 
unsigned int currentLcdLightOnTime = 0;  // For calculating the lcd backlight on time.  
unsigned long lcdLightOn_StartMillis;  
boolean isLcdLightOn;
 
int buttonState = 0;  // For checking push-button state.

CapacitiveSensor   cs_7_6 = CapacitiveSensor(7,6);        // 10M resistor between pins 7 & 6, pin 6 is sensor pin
LiquidCrystal lcd(9, 8, 5, 4, 3, 2);
//byte p1[8] = {
//  0x10,
//  0x10,
//  0x10,
//  0x10,
//  0x10,
//  0x10,
//  0x10,
//  0x10};
// 
//byte p2[8] = {
//  0x18,
//  0x18,
//  0x18,
//  0x18,
//  0x18,
//  0x18,
//  0x18,
//  0x18};
// 
//byte p3[8] = {
//  0x1C,
//  0x1C,
//  0x1C,
//  0x1C,
//  0x1C,
//  0x1C,
//  0x1C,
//  0x1C};
// 
//byte p4[8] = {
//  0x1E,
//  0x1E,
//  0x1E,
//  0x1E,
//  0x1E,
//  0x1E,
//  0x1E,
//  0x1E};
// 
//byte p5[8] = {
//  0x1F,
//  0x1F,
//  0x1F,
//  0x1F,
//  0x1F,
//  0x1F,
//  0x1F,
//  0x1F};

void setup() 
{
   cs_7_6.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);
//   lcd.createChar(0, p1);
//   lcd.createChar(1, p2);
//   lcd.createChar(2, p3);
//   lcd.createChar(3, p4);
//   lcd.createChar(4, p5);
   lcd.begin(16, 2);
   lcd.print("Water Level:");
   lcd.setCursor(0, 1);
   lcd.print("E");
   lcd.setCursor(15, 1);
   lcd.print("F");

//   pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);      // Set the push-button pin as an input.
  pinMode(LCD_LIGHT_PIN, OUTPUT);    // Set the lcd display backlight anode pin as an output.
  digitalWrite(LCD_LIGHT_PIN, LOW);    // Set the lcd display backlight anode pin to low (lcd light off)

  isLcdLightOn = false;

  Ethernet.begin(mac);
  delay(1000);
  Serial.println("connecting...");
}

void loop()                    
{
    // Check the state of the push-button.
  buttonState = digitalRead(BUTTON_PIN);
 
  if (buttonState == HIGH){      // Button pressed.
    Serial.println("Button pressed - HIGH"); 
    lcdLightOn_StartMillis = millis();
    currentLcdLightOnTime = 0;
    isLcdLightOn = true;
    digitalWrite(LCD_LIGHT_PIN, HIGH);
  }
  else{    // Button not pressed.
    if(isLcdLightOn){
      currentLcdLightOnTime = millis() - lcdLightOn_StartMillis;
      if(currentLcdLightOnTime > LCD_LIGHT_ON_TIME){
        isLcdLightOn = false;
        digitalWrite(LCD_LIGHT_PIN, LOW);
      }
    }
  }
   
    long start = millis();
    long capacitance =  cs_7_6.capacitiveSensorRaw(100);  //collects n samples and returns sum
//    long waterlevel = capacitance-12650; //Do this adjustment first: subtract n to bring the reported value down close to 0 when empty
//    waterlevel = waterlevel/180;  //Only do this after above step complete: Divide by n to bring value reported down to 100 when full
    long waterlevel = capacitance-16100; //Do this adjustment first: subtract n to bring the reported value down close to 0 when empty
    waterlevel = waterlevel/600;  //Only do this after above step complete: Divide by n to bring value reported down to 100 when full
    String waterLvlPct = String(waterlevel);
    int strlength = waterLvlPct.length();

//    Serial.print(capacitance);        // print raw output of capacitance reading
//    Serial.print("\t");                    // tab character for debug window spacing
//    Serial.println(waterlevel);                  // print water sensor reading

  //Print water level output to LCD:
  lcd.setCursor(5,1);
  lcd.print(waterlevel);

//  double a=LCDlength/100*waterlevel;

 // drawing black rectangles on LCD
 
//  if (a>=1) {
//    for (int i=1;i<a;i++) {
//      lcd.write(4);
//      b=i;
//    }
//    a=a-b; 
//  }
// 
//  peace=a*5;
////clearing line
//  for (int i =0;i<(LCDlength-b);i++) {
//    lcd.print(" ");
//  }
//  ;
//  
//// drawing charater's colums
// 
//  switch (peace) {
//  case 0: 
//    break;
//  case 1:
//    lcd.print((char)0); 
//    break;
//  case 2:
//    lcd.write(1);
//    break;
//  case 3:
//    lcd.write(2);
//    break;
//  case 4:
//    lcd.write(3);
//    break;
//  }

  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected)
  {
    Serial.println();
    Serial.println("...disconnected.");
    Serial.println();    
    client.stop();
  }
  
  // Update ThingSpeak
  if(!client.connected() && (millis() - lastConnectionTime > updateInterval))
  {
    updateThingSpeak("field1="+waterLvlPct);
  }  
  lastConnected = client.connected();
}

void updateThingSpeak(String tsData)
{
  if (client.connect(server, 80))
  { 
    Serial.println("Connected to ThingSpeak...");
    Serial.println();
        
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);

    lastConnectionTime = millis();
    
    resetCounter = 0;

  }
  else
  {
    Serial.println("Connection Failed.");   
    Serial.println();
    resetCounter++;
    if (resetCounter >=5) {
      resetEthernetShield();
    }
    lastConnectionTime = millis(); 
  }
}

void resetEthernetShield()
{
  Serial.println("Resetting Ethernet Shield.");   
  Serial.println();
  
  client.stop();
  delay(1000);
  
  Ethernet.begin(mac);
  delay(1000);
}
