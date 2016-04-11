//basic pulse sensor demo for atmega328

//Neopixels settings
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 18
#define RESOLUTION 1024.0
#define NEOPIN 6
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

//we're powering the pulse sensor from GPIO pins for convenience (watch for noise)
int pulsePower= A1;
int pulseGnd= A2;
int pulsePin= A0;


void setup() {
  //set up power for pulse sensor
  digitalWrite(pulseGnd, LOW);
  digitalWrite(pulsePower, HIGH);
  digitalWrite(pulsePin, LOW);
  pinMode(pulseGnd, OUTPUT);
  pinMode(pulsePower, OUTPUT);
  pinMode(pulsePin, INPUT);

  pinMode(13, OUTPUT);  //additional blinky
  Serial.begin(115200);
  pixels.begin();
  
}
int nsamples = 100; //a cheap first order filter (the tank is n times bigger than a single sample)
long int lowpass = 500*nsamples;  //init the filter to about the bias level of the sensor

void loop() {
  int level = analogRead(pulsePin); //fetch a sample
  //print pretty debug messages
  Serial.print(level);
  Serial.print(", ");
  Serial.print(lowpass/nsamples);
  Serial.print(", ");

  //the length of a horizontal bar graph
  int leng = NUMPIXELS*(level/RESOLUTION);
  
  for(int i=0; i<leng; i++){  //print this many YES blocks
    Serial.print('#');
    pixels.setPixelColor(i, pixels.Color(128,0,0)); // Moderately bright red color.
  }
  for(int i=leng; i<NUMPIXELS; i++){ //print this many NO blocks
    Serial.print('-');
    pixels.setPixelColor(i, pixels.Color(0,30,0)); // dim green color.
  }

  pixels.show();
  Serial.println("");

  lowpass += level;  //add a sample to the accumulator
  lowpass = lowpass* ((float)nsamples/((float)nsamples + 1.0)); take the weighted mean of the accumulator + sample

  digitalWrite(13, ((level>(lowpass/nsamples))?HIGH:LOW));  //switch the LED on if the current level exceeds the mean
  delay(10);
}

