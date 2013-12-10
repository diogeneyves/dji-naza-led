
#include <Adafruit_NeoPixel.h>
#include <math.h>
#define PIN 7
#define ROTL(v, n) (v << n) | (v >> (sizeof(v) * 8 - n))
#define ROTR(v, n) (v >> n) | (v << (sizeof(v) * 8 - n))
#define RADIOIN1 13
#define RADIOIN2 14
#define FADING_STEP 5
#define BRIGHT 30 //  brightness 0-255  
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(64, PIN);
word statesL = 0x303; // two arm define
word statesR = 0x303; // two arm define
//word statesL = 0xC63; // tree arm define
//word statesR = 0xC63; // tree arm define
long startTime;
int sensorValue0;  // value coming from the Naza equal led green
int sensorValue1;  // value coming from the Naza equal led red
int duration; // read throtle
int radiomin; // minimum throtle
int turn = 100; // how fat turn led
long color; // end color
byte select; // select mode animation
const int turns[] = {
  0, 0, 0, 0, 0, 0, 320, 160, 80, 40, 20, 10}; // value by step 200 radio

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(30); //  brightness 0-255
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(RADIOIN1, INPUT);
  pinMode(RADIOIN2, INPUT);
  //radiomin = pulseIn(RADIOIN, HIGH, 25000); // store min throtle
}

void loop() {
  selectMode();
  colors();
  time();
  radio();
}

void radio() {
  duration = pulseIn(RADIOIN1, HIGH, 25000);  // how fast led turn
  int d = (int)ceil((float)duration / 200.0f);
  turn = turns[((d <= 11) ? d : 11)];
}

void ring() {
  int i;
  int stateL;
  int stateR;
  if (turn == 0) {
    neutre();
  }
  else {
    for (i = 0; i < 16; i++) {
      stateL = (statesL >> i) & 0x0001;
      stateR = (statesR >> i) & 0x0001;
      pixels.setPixelColor(i, stateR * color);
      pixels.setPixelColor(i+16, stateL * color);
      pixels.setPixelColor(i+32, stateR * color);
      pixels.setPixelColor(i+48, stateL * color);
    }
  }
  statesL = ROTL(statesL, 1);
  statesR = ROTR(statesR, 1);
  //color = ROTL(color, 8); // change color
  pixels.show();
}

void time() {
  if (millis() - startTime >= turn) {
    switch (select) {
    case 1:
      ring();
      break;
    case 2:
      fading();
      break;
    }
    startTime = millis();
  }
}

void colors() {
  sensorValue0 = analogRead(A4); // read Naza red led
  sensorValue1 = analogRead(A5); // read Naza green led
  byte ledV = (sensorValue0 > 500);
  byte ledR = (sensorValue1 > 500)<<1;
  switch (ledR + ledV) {
  case 0:
    if (turn == 0) {
      color = 0x000000; //none
    }
    break;
  case 1:
    color = 0x00FF00; //green
    break;
  case 2:
    color = 0xFF0000; //red
    break;
  case 3:
    color = 0xFF9900; //orange
    break;
  }
}

void fading() {
  int i;
  static int direction = 1; // start direction fading
  static int intensity = 0;

  if (turn == 0) { // throtle minimum
    neutre();
  }
  else {
    for (i = 0; i < 64; i++) 
      pixels.setPixelColor(i, color);
    if ( direction == 1) { 
      intensity += FADING_STEP;               
    }
    if ( intensity > 250 ) { 
      direction = -1;               
    }
    if ( direction == -1) { 
      intensity -= FADING_STEP;               
    }
    if ( intensity < 5 ) { 
      direction = 1;               
    }
    pixels.setBrightness(intensity); //  brightness
  }
  pixels.show();
}

void selectMode() {
  select = pulseIn(RADIOIN2, HIGH, 25000);
  if (select < 1500) {
    select = 1;
  }
  else {
    select = 2;
  }
}

void neutre() {
  int i;
  for (i = 0; i < 16; i++) {
    pixels.setPixelColor(i, color);
    pixels.setPixelColor(i+16, color);     
    pixels.setPixelColor(i+32, color);     
    pixels.setPixelColor(i+48, color); 
    pixels.setBrightness(BRIGHT); //  brightness 0-255  
  }
}

