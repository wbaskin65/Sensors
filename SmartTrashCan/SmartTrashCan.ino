#include <WiFi.h>        // For connecting ESP32 to WiFi
#include <ArduinoOTA.h>  // For enabling over-the-air updates
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <Wire.h>

const char* ssid = "Will iPhone";  // Change to your WiFi Network name
const char* password = "Baskin65";  // Change to your password

WiFiClient  client;

int pinState = 1;

unsigned long myChannelNumber = 2770368;
const char * myWriteAPIKey = "G0YX75GM0QGJNPGN";

const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration;

// Initialize our values
int test = 0;
String myStatus = "Working";

double pr3 = 66.4;


void setup() {
  analogReadResolution(12);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);  // Connect to WiFi - defaults to WiFi Station mode

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(35, INPUT);

  // Ensure WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Not connected");
  }

  ArduinoOTA.begin();  // Starts OTA
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

//Kalman variables
double q = 0.1;   // Process noise
double r = 1.0;   // Measurement noise
double x = 0.0;   // Starting estimate
double p = 1.0;   // Starting error estimate
double k = 0.0;   // Gain, set to 0 to start with

double kalman(double measurement){
    //Kalman filter application

    // Updated prediction
    p = p + q;

    // Measurement update
    k = p / (p + r);
    x = x + k * (measurement - x);
    p = (1 - k) * p;

    return x;
}

void loop() {
  int hallRead = analogRead(35);
  Serial.println(hallRead);

  if (hallRead >= 3100 || hallRead <= 2200) {
    // CLOSE
    pinState = 0;
  }


  if (hallRead >= 2750 && hallRead <= 2950) {
    // FAR AWAY
    pinState = 1;
  }

  // Hall sensor outputs
  if (pinState == 0) {
    Serial.printf("HIGH\n");
  }
  else if (pinState == 1) {
    Serial.printf("LOW\n");
  }
  else {
    Serial.printf("something went wrong\n");
  }


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
  Serial.printf("Distance (cm): ");
  Serial.println(usdistance);


  int fsrRead = analogRead(34);
  Serial.printf("FSR: ");
  Serial.println(fsrRead);

  double vfsrp = (fsrRead + 160.0)/1245.0;
  double fsrRp = -(62.0*vfsrp)/(vfsrp-5.0); //Transfer function to get resistance of FSR and linearizing resistor in parallel
  double fsrR = -(pr3 * fsrRp)/(fsrRp - pr3); //Finds resistance of FSR alone
  Serial.printf("FSR resistance: ");
  Serial.println(fsrR);

  double scaledExponent = 500.0 / 371.0; // Reduced exponent
  int force = (2.159 * pow(10.0,9)) / (211.0 * pow(fsrR, scaledExponent));

  force = kalman(force);

  if (force < 600){
    force = 0;
  }

  Serial.print("Force (g): ");
  Serial.println(force);


  ArduinoOTA.handle();  // Handles a code update request
  // All loop you're code goes here.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Not working");
  }

  int capacity = 100 - (usdistance/18)*100;

  // set the fields with the values
  ThingSpeak.setField(1, pinState);
  ThingSpeak.setField(2, capacity);
  ThingSpeak.setField(3, force);
 
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
 
  delay(10000); // Wait 10 seconds to update the channel again
}