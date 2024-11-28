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

#include <M5_PbHub.h>
M5_PbHub myPbHub;

int maLectureKeyPrecedente;
int etatPlay;
int numberOfClick;
int red;
int green;

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

  Wire.begin();
  myPbHub.begin();
  myPbHub.setPixelCount(CHAN_KEY, 1);
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
void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  //monOsc.onOscMessageReceived(maReceptionMessageOsc);

  if (millis() - monChronoMessage >= 25) {
    monChronoMessage = millis();
    // à chaque 50ms
    bool maLectureBouton = M5.Btn.isPressed();
    //Serial.println(maLectureBouton);
    
    //int maLectureAngle = analogRead(MA_BROCHE_ANGLE);
    /*
    Serial.print(maLectureBouton);
    Serial.print(" ");
    Serial.println(maLectureAngle); //fin ligne
    */
    //monOsc.sendInt("/bouton", maLectureBouton);

/*
    int maLectureKey = myPbHub.digitalRead(CHAN_KEY);

    

    if (maLectureKeyPrecedente != maLectureKey) {
      if (maLectureKey == 0) {
        monOsc.sendInt("/key", maLectureKey);
        etatPlay = !etatPlay;
        monOsc.sendInt("/numberOfClick", numberOfClick);

        maBandeDel[numberOfClick] = CRGB(red, green, 0);
        FastLED.show();

        numberOfClick += 1;
        if (numberOfClick >= random(8, 28)) {
          red = 100;
          green = 0;
        }
        if (numberOfClick == 30) {
          for (int i=0; i < LONGUEUR; i++) {
            maBandeDel[i] = CRGB(0, 0, 0);
          }
          FastLED.show();
          numberOfClick = 0;
          red = 0;
          green = 100;
        }
        /*
        if (etatPlay == 1) {
          
        } else {
          
        }
        */
        /*
      }
    }
    maLectureKeyPrecedente = maLectureKey;
*/
  }


  if (millis() - monChronoMessageDeux >= 600) {
    monChronoMessageDeux = millis();
    //lightsOn = !lightsOn;
    
    if (lightsOn == 0) {
      for (int i=0; i < LONGUEUR; i++) {
        maBandeDel[i] = CRGB(random(5, 50), 0, random(0, 20));
      }
      FastLED.show();
    } else {
      for (int i=0; i < LONGUEUR; i++) {
        maBandeDel[i] = CRGB(0, 0, 0);
      }
      FastLED.show();
    }
  }

}