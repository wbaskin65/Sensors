#include <WiFi.h>        // For connecting ESP32 to WiFi
#include <ArduinoOTA.h>  // For enabling over-the-air updates
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include  <Adafruit_BMP280.h>
#include <Wire.h>

float bias = 2.3603;
float gain = 1.2491;

double WR = 180.0;

double PR3 = 340.0;

int FSRpin = 36;
int thermPin = 34;

int ad590Pin = 33; // Analog pin connected to AD590 output
float resistorValue = 10000.0; // 10k Ohm resistor

int LDRpin = 32;

const char* ssid = "Will iPhone";  // Change to your WiFi Network name
const char* password = "Baskin65";  // Change to your password


WiFiClient  client;

int pinState = 1;

unsigned long myChannelNumber = 2647477;
const char * myWriteAPIKey = "EAM6YKBD274OY116";


// Initialize our values
int test = 0;
long pressure;
String myStatus = "Working";

void setup() {
  analogReadResolution(12);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);  // Connect to WiFi - defaults to WiFi Station mode

  pinMode(FSRpin, INPUT);
  pinMode(ad590Pin, INPUT);

  // Ensure WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Not connected");
  }


  ArduinoOTA.begin();  // Starts OTA
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}


void loop() {
  int analogValue = analogRead(thermPin);
  Serial.println(analogValue);
  
  double Vreal = (analogValue + 160)/1245.0; //Calculates voltage from adcvalue

  Serial.print("Vreal: ");
  Serial.println(Vreal);
  
  double removeBias = Vreal - bias; //Removes voltage bias

  double removeGain = removeBias/gain; //Removes gain adjustment

  double removeWheatstone = (WR*(removeGain+(5.0/2.0)))/((5.0/2.0)-removeGain); //Inverse of wheatstone bridge function

  double remove3PR = (removeWheatstone*PR3)/(PR3-removeWheatstone); //Finds initial resistance of thermistor

  double betaTransfer = (1084073.4)/(3636.0 + 298.15*log(0.001*remove3PR)); //Uses beta model of thermistor to find temperature based on resistance

  double thermTemp = betaTransfer - 273.15;

  Serial.print("Temp in kelvin: ");
  Serial.println(betaTransfer);

  delay(20);

  int FSR = analogRead(FSRpin);
  Serial.print("FSR: ");
  Serial.println(FSR);

  int ad590adc = analogRead(ad590Pin);
  double ad590v = (ad590adc + 160)/1245.0;
  float current = ad590v / resistorValue; // Calculate current in Amperes
  float temperatureK = current * 1*pow(10,6) + 15; // Convert current to temperature in Kelvin (1 µA/K + 90K)
  float temperatureC = temperatureK - 273.15; // Convert Kelvin to Celsius

  Serial.print("AD590 Kelvin Temp: ");
  Serial.println(temperatureK);

  int LDR = analogRead(LDRpin);
  Serial.print("LDR: ");
  Serial.println(LDR);


  ArduinoOTA.handle();  // Handles a code update request
  // All loop you're code goes here.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Not working");
  }


  // set the fields with the values
  ThingSpeak.setField(1, test);
  ThingSpeak.setField(2, temperatureC);
  ThingSpeak.setField(3, float(thermTemp));
 
  // set the status
  ThingSpeak.setStatus(myStatus);
 
  if (FSR < 3500 || LDR > 2000) {// write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }
 
  delay(5000); // Wait 20 seconds to update the channel again
}