/* Pulse Sensor Amped with NeoPixels
 
Pulse an arbitrary number of Adafruit NeoPixels based on a heartbeat sensor
 
The pulsesensor.com code needs to be in module interrupt.ino in the sketch directory
   http://pulsesensor.com/pages/pulse-sensor-amped-arduino-v1dot1
Code also uses the Adafruit NeoPixel library code discussed at
   https://learn.adafruit.com/adafruit-neopixel-uberguide
 
Version 1.0 by Mike Barela for Adafruit Industries, Fall 2015
Modified by BrettRD and ZestyMilk for the ATTiny85
*/
#include <Adafruit_NeoPixel.h>    // Library containing 

// Behavior setting variables
int pulsePin = A0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 0;                 // Digital pin to blink led at each beat
int fadePin  = 4;                 // pin to do fancy neopixel effects at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin
 
// these variables are volatile because they are used during the interrupt service routine
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, the Inter-Beat Interval
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
 
// Set up use of NeoPixels
const int NUMPIXELS = 9;           // Put the number of NeoPixels you are using here
const int BRIGHTNESS = 250;          // Set brightness of NeoPixels here
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, fadePin, NEO_GRB + NEO_KHZ800);
 
void setup(){
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  //Serial.begin(115200);           // Serial output data for debugging or external use
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  for (int x=0; x < NUMPIXELS; x++) {  // Initialize all pixels to 'off'
     strip.setPixelColor(x, strip.Color(0, 0, 0));
  }
  strip.show();                     // Ensure the pixels are off 
  delay(1000);                      // Wait a second
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
}
 
void loop(){
  sendDataSerial('S', Signal);      // send Processing the raw Pulse Sensor data
  if (QS == true){                    // Quantified Self flag is true when arduino finds a heartbeat
     fadeRate = 250;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
//     sendDataSerial('B',BPM);       // send heart rate with a 'B' prefix
//     sendDataSerial('Q',IBI);       // send time between beats with a 'Q' prefix
     QS = false;                      // reset the Quantified Self flag for next time    
  }
  ledFadeToBeat();                    // Routine that fades color intensity to the beat
  delay(20);                          //  take a break
}
 
void ledFadeToBeat() {
    fadeRate -= 15;                         // Set LED fade value
    fadeRate = constrain(fadeRate,0,150);   // Keep LED fade value from going into negative numbers
    setStrip(fadeRate);                     // Write the value to the NeoPixels 
    sendDataSerial('R',fadeRate);
}
 
void sendDataSerial(char symbol, int data ) {
//    Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
//    Serial.println(data);                // the data to send culminating in a carriage return
}
 
void setStrip(int r) {     // Set the strip to one color intensity (red)
   int g = 0;              // Green is set to zero (for non-red colors, change this)
   int b = 20;              // Blue is set to zero (for non-red colors, change this)
   for (int x=0; x < NUMPIXELS; x++) {
      strip.setPixelColor(x, strip.Color(b, (r/2.5), ((b*2)+r)));
   }
   strip.show();
}
