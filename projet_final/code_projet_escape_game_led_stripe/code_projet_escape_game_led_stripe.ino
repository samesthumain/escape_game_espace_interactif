#include <M5Atom.h>
#include <FastLED.h>

CRGB monPixelAtom[1];  // int bob = 10;
unsigned long monChronoMessage; //commence automatiquement à 0
unsigned long monChronoMessageDeux; //commence automatiquement à 0

#define CHAN_KEY 2

//#define MA_BROCHE_ANGLE 32 //const int MA_BROCHE_ANGLE = 32;

int lightsOn;

#define LONGUEUR 30
CRGB maBandeDel[LONGUEUR];

#include <MicroOscSlip.h>
MicroOscSlip<128> monOsc(&Serial);

int maLectureKeyPrecedente;
int etatPlay;
int numberOfClick;
int red;
int green;
int newCounter = 0;

void setup() {
  // put your setup code here, to run once:
  M5.begin(false, false, false);
  Serial.begin(115200);  //demare communication avec ordi

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(monPixelAtom, 1);  // Ajouter le pixel du M5Atom à FastLED
  

  FastLED.addLeds<WS2812, 26, RGB>(maBandeDel, LONGUEUR);

  //NeoPixel.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  unsigned long chronoDepart = millis();
  while (millis() - chronoDepart < 5000) {
    monPixelAtom[0] = CRGB(1,255,251);
    FastLED.show();
    delay(100);
    
    monPixelAtom[0] = CRGB(0,0,0);
    FastLED.show();
    delay(100);
  }

  lightsOn = 0;
  numberOfClick = 0;
  red = 0;
  green = 100;

  //Wire.begin();
  //myPbHub.begin();
  //myPbHub.setPixelCount(CHAN_KEY, 1);
}

/*
void maReceptionMessageOsc(MicroOscMessage& oscMessage) {
  if (oscMessage.checkOscAddress("/rgb")) {
    int r = oscMessage.nextAsInt();
    int g = oscMessage.nextAsInt();
    int b = oscMessage.nextAsInt();
    pixel = CRGB(r,g,b);
    FastLED.show();
  }
}
*/

void maReceptionMessageOsc(MicroOscMessage& oscMessage) {
  if (oscMessage.checkOscAddress("/counter")) {
    int counter = oscMessage.nextAsInt();
    monOsc.sendInt("/compteurLight", counter);
    maBandeDel[counter] = CRGB(0, 100, 0);
    FastLED.show();
    if (newCounter != counter) {
      if (counter == 0 && newCounter > 0) {
        for (int i=0; i < LONGUEUR; i++) {
          maBandeDel[i] = CRGB(0, 0, 0);
          FastLED.show();
        }
      } if (counter > 0) {
        maBandeDel[counter] = CRGB(0, 100, 0);
        FastLED.show();
      }
      newCounter = counter;
    }
  }
  /*
  if (oscMessage.checkOscAddress("/resetClick")) {
    int reset = oscMessage.nextAsInt();
    monOsc.sendInt("/counterReset", reset);
    if (reset == 1) {
      for (int i=0; i < LONGUEUR; i++) {
        maBandeDel[i] = CRGB(0, 0, 0);
      }
    }
  }
  */
}


void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  monOsc.onOscMessageReceived(maReceptionMessageOsc);
  

  if (millis() - monChronoMessage >= 25) {
    monChronoMessage = millis();

    

    

    // à chaque 50ms
    bool maLectureBouton = M5.Btn.isPressed();
    //Serial.println(maLectureBouton);
  }
    

}