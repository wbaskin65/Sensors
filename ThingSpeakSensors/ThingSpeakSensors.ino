#include <WiFi.h>        // For connecting ESP32 to WiFi
#include <ArduinoOTA.h>  // For enabling over-the-air updates
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include  <Adafruit_BMP280.h>
#include <Wire.h>

Adafruit_BMP280 bmp; // I2C Interface


const char* ssid = "HD-iPhone-14-128GB";  // Change to your WiFi Network name
const char* password = "chaverim";  // Change to your password


WiFiClient  client;

int pinState = 1;

unsigned long myChannelNumber = 2647477;
const char * myWriteAPIKey = "EAM6YKBD274OY116";

const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration;


// Initialize our values
int test = 0;
long pressure;
String myStatus = "Working";

void setup() {
  analogReadResolution(12);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);  // Connect to WiFi - defaults to WiFi Station mode

  bmp.begin();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(15, INPUT);

  // Ensure WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Not connected");
  }


  ArduinoOTA.begin();  // Starts OTA
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}


void loop() {
  int analogValue = analogRead(35);
  Serial.println(analogValue);

  pressure = bmp.readPressure();
  Serial.println(pressure);



  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  float usdistance = duration * SOUND_SPEED/2;
  Serial.println(usdistance);



  ArduinoOTA.handle();  // Handles a code update request
  // All loop you're code goes here.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Not working");
  }


  // set the fields with the values
  ThingSpeak.setField(1, test);
  ThingSpeak.setField(2, usdistance);
  ThingSpeak.setField(3, analogValue);
  ThingSpeak.setField(4, pressure);
 
  // set the status
  ThingSpeak.setStatus(myStatus);
 
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
 
  delay(5000); // Wait 20 seconds to update the channel again
}
