/*CODE REFERENCES:
  Capacitive: https://playground.arduino.cc/Main/CapacitiveSensor/
  Motors:
*/

#include <CapacitiveSensor.h>
#include <Adafruit_NeoPixel.h>

//Capacitive Sensor Variables
CapacitiveSensor cs1 = CapacitiveSensor(6, 7); //initialize pin as capacitive sensor

//Small Cell Motor Variables
int motorPin = 10;                    //Output pin on Arduino

//Party Fan (Confetti)
int fanPin = 9;                    //This is the output pin on the Arduino

//define Neopixel pin
#define LED_PIN 5
#define LED_COUNT 20  //define # of NeoPixels
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//emotion variables
int sadVar;     //variable for sadness mapping
int happyVar;   //variable for happiness mapping


int inByte;


void setup() {
  Serial.begin(9600);

  //Capacitive
  pinMode(8, INPUT_PULLUP);           //initialize pin as internal pull-up

  //Cell Motors
  pinMode(motorPin, OUTPUT);          //set pin as output

  //Part Fan (Confetti)
  pinMode(fanPin, OUTPUT);            //Sets that pin as an output

  // NeoPixel Code
  strip.begin();            //initialze NeoPixel strip (REQUIRED)
  strip.show();             //Turn OFF all pixels ASAP
  strip.setBrightness(50);  //Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  if (Serial.available() > 0) {   // if there's serial data available
    inByte = Serial.read();       // read it
    Serial.write(inByte);         // send it back out as raw binary data
  }
  // Serial.println(inByte);

  // Capacitive sensor
  long cs1v = cs1.capacitiveSensor(80);
    Serial.println(cs1v);
  delay(15);

//  //Capacitive with motor control
//  if (cs1v < 500 && inByte <= 7) {
//    digitalWrite(motorPin, HIGH);       //Switch Motor ON
//    delay(2000);                        //Wait 2 Second
//    digitalWrite(motorPin, LOW);        //Switch Motor OFF
//    delay(1000);                        //Wait 1 Second
//  } else if (cs1v < 500 && inByte > 7) {
//    digitalWrite(motorPin, LOW);        //Shut off motor
//  }
  //Capacitive with motor control
  if (cs1v > 500) {
    digitalWrite(motorPin, HIGH);       //Switch Motor ON
    delay(2000);                        //Wait 2 Second
    digitalWrite(motorPin, LOW);        //Switch Motor OFF
    delay(1000);                        //Wait 1 Second
  } else if (cs1v < 500) {
    digitalWrite(motorPin, LOW);        //Shut off motor
  }

  //confetti fan
  if (inByte == 16) {
    //Confetti Fan
    digitalWrite(fanPin, HIGH);           //Switch Fan ON
    delay(5000);                          //Wait 1 Second
    digitalWrite(fanPin, LOW);            //Switch Fan OFF
    delay(3000);
  } else if (inByte > 16) {
    digitalWrite(fanPin, LOW);       //Switch Fan OFF
  }


  sadVar = map(inByte, 10, 6, 100, 255);
  happyVar = map(inByte, 12, 16, 100, 255);



  if (inByte == 12) {
    sadVar = 0;
  }

  uint32_t red = strip.Color(sadVar, 0, 0); //Establish red color
  uint32_t green = strip.Color(0, happyVar, 0); //Establish green color
  uint32_t fuschia = strip.Color(255, 128, 255); //Establish fuschia color
  uint32_t yellow = strip.Color(255, 255, 0); //Establish yellow color

  if (inByte == 16) {
    strip.fill(fuschia);        //fill strip with fuschia
  } else if (inByte < 16 && inByte >= 12) {
    strip.fill(green);        //fill strip with green
  } else if (inByte == 11) {
    strip.fill(yellow);        //fill strip with yellow
  } else if (inByte <= 10) {
    strip.fill(red);          //fill strip with red
  }
  strip.show(); //push strip to be updated


  //Conditionals to control then mapping goes negative
  if (inByte > 10) {
    sadVar == 0;
  } else if (inByte == 11) {
    happyVar = 0;
    sadVar = 0;
  } else if (inByte < 12) {
    happyVar == 0;
  }

}

