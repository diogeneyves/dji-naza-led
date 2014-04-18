/*

 An Open Source Arduino based DJI Led project.
 
 Program : DJI Led (Firmware)
 Version : V1.1, Decembre 11th 2013
 Author(s): Yves Gohy (diogeneyves@gmail.com)
 Thanks to: Leandre Gohy (leandre.gohy@hexeo.be)
 
 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 For more detail on the MIT license, see <http://opensource.org/licenses/MIT>
 
 */

#include <Adafruit_NeoPixel.h>
#include <math.h>
#define PIN 9
#define BRIGHTNESS 30 // brightness 0-255  
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(64, PIN);
int animationSpeed = 0;

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(BRIGHTNESS);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

unsigned long int getAnimationColor(word animationSpeed) {
  static const unsigned long int animationColors[] = { 
    0x000000, 0x00FF00, 0xFF0000, 0xFF9900           }; // colors of animation
  static unsigned long int animationColor = 0x000000;
  // read Naza red led
  int sensorValue0 = analogRead(A2);
  // read Naza green led
  int sensorValue1 = analogRead(A3);
  byte animationColorIndex = (sensorValue0 > 500) + ((sensorValue1 > 500) << 1);

  if (animationColorIndex != 0 || animationSpeed == 0) {
    animationColor = animationColors[animationColorIndex];
  }
  return animationColor;
}

void playNoAnimation(unsigned long int animationColor) {
  for (byte i = 0; i < 16; i++) {
    pixels.setPixelColor(i, animationColor);
    pixels.setPixelColor(i+16, animationColor);
    pixels.setPixelColor(i+32, animationColor);
    pixels.setPixelColor(i+48, animationColor);
    pixels.setBrightness(BRIGHTNESS);
  }

  pixels.show();
}

void loop() {
  unsigned long int animationColor = getAnimationColor(animationSpeed);
  playNoAnimation;
}





