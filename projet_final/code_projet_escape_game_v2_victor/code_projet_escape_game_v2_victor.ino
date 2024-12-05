#include <M5Atom.h>
#include <FastLED.h>

CRGB monPixelAtom[1];  // int bob = 10;
unsigned long monChronoMessage; //commence automatiquement à 0
unsigned long monChronoMessageDeux; //commence automatiquement à 0

#define CHAN_KEY 3
#define CHAN_ANGLE 1
#define CHAN_KEY_COOL 2
#define CHAN_KEY_SUBMIT 4

//#define MA_BROCHE_ANGLE 32 //const int MA_BROCHE_ANGLE = 32;

int lightsOn;

/*
#define LONGUEUR 30
CRGB maBandeDel[LONGUEUR];
*/

#include <MicroOscSlip.h>
MicroOscSlip<128> monOsc(&Serial);

#include <M5_PbHub.h>
M5_PbHub myPbHub;

#include <VL53L0X.h>
VL53L0X myTOF;

#include "Unit_Encoder.h"
Unit_Encoder myEncoder;
 
int myEncoderPreviousRotation;

int maLectureKeyPrecedente;
int maLectureKeyPrecedenteCool;
int maLectureKeyPrecedenteSubmit;

int etatPlay;
int numberOfClick;
int red;
int green;

void setup() {
  // put your setup code here, to run once:
  M5.begin(false, false, false);
  Serial.begin(115200);  //demare communication avec ordi

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(monPixelAtom, 1);  // Ajouter le pixel du M5Atom à FastLED
  

  //FastLED.addLeds<WS2812, 26, RGB>(maBandeDel, LONGUEUR);

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

  myEncoder.begin(); // Démarrer la connexion avec l'encodeur

  myTOF.init();
  myTOF.setTimeout(500);
  myTOF.startContinuous();

  
  myPbHub.begin();
  myPbHub.setPixelCount(CHAN_KEY, 1);

  monOsc.sendInt("/boundesStart", 1600);
  monOsc.sendInt("/boundesEnd", 2500);
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

    int maLectureAngle = myPbHub.analogRead(CHAN_ANGLE);
    //int tune = maLectureAngle / 4095.0 * 127;
    monOsc.sendInt("/angle", maLectureAngle);

    
    uint16_t millimetres = myTOF.readRangeContinuousMillimeters();
    //int pos =  millimetres/ 500.0 * 127;
    //int pos = map(millimetres, 0, 1200, 0, 127);
    int amount = 0;
    monOsc.sendInt("/tof", millimetres);
    if(millimetres <= 500){
      monOsc.sendInt("/appear", 0);
    }else{
      monOsc.sendInt("/appear", 1);
    }
    

    int maLectureKey = myPbHub.digitalRead(CHAN_KEY);

    if (maLectureKeyPrecedente != maLectureKey) {
      if (maLectureKey == 0) {
        monOsc.sendInt("/key", maLectureKey);
        etatPlay = !etatPlay;

        
        /*
        maBandeDel[numberOfClick] = CRGB(red, green, 0);
        FastLED.show();
        */
        
        /*
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
        
      }
      
    }
    maLectureKeyPrecedente = maLectureKey;

    int maLectureKeyCool = myPbHub.digitalRead(CHAN_KEY_COOL);

    if (maLectureKeyPrecedenteCool != maLectureKeyCool) {
      if (maLectureKeyCool == 0) {
        monOsc.sendInt("/keyCount", maLectureKeyCool);
        etatPlay = !etatPlay;
        numberOfClick += 1;
        monOsc.sendInt("/numberOfClick", numberOfClick);

        if (numberOfClick == 32) {
          numberOfClick = 0;
        }
        /*
        maBandeDel[numberOfClick] = CRGB(red, green, 0);
        FastLED.show();
        */
        
        /*
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
        
      }
      
    }
    maLectureKeyPrecedenteCool = maLectureKeyCool;


    int maLectureKeySubmit = myPbHub.digitalRead(CHAN_KEY_SUBMIT);
    if (maLectureKeyPrecedenteSubmit != maLectureKeySubmit) {
      if (maLectureKeySubmit == 0) {
        int randomNumber = random(1200, 3000);
        int randomNumberTwo = randomNumber + 900;
        if (numberOfClick != 12 || numberOfClick == 0){
          if (numberOfClick == 0) {
            monOsc.sendInt("/echec", 0);
          }
          monOsc.sendInt("/boundesStart", randomNumber);
          monOsc.sendInt("/boundesEnd", randomNumberTwo);
          monOsc.sendInt("/resetClick", 1);
          monOsc.sendInt("/numberOfClick", 0);
          monOsc.sendInt("/succes", 0);
          monOsc.sendInt("/echec", 1);
          numberOfClick = 0;
        } else if (numberOfClick == 12) {
          monOsc.sendInt("/boundesStart", randomNumber);
          monOsc.sendInt("/boundesEnd", randomNumberTwo);
          monOsc.sendInt("/succes", 1);
          monOsc.sendInt("/numberOfClick", 0);
          monOsc.sendInt("/echec", 0);
          numberOfClick = 0;
        }
      }
    }
    maLectureKeyPrecedenteSubmit = maLectureKeySubmit;

    
  }


  /*if (millis() - monChronoMessageDeux >= 7000) {
    monChronoMessageDeux = millis();
    if (numberOfClick != 12 || numberOfClick == 0){
      monOsc.sendInt("/resetClick", 1);
      monOsc.sendInt("/numberOfClick", 0);
      numberOfClick = 0;
    }
  }*/

  /*
  if (millis() - monChronoMessageDeux >= 7000) {
    monChronoMessageDeux = millis();
    if (numberOfClick != 12 || numberOfClick == 0){
      monOsc.sendInt("/resetClick", 1);
      monOsc.sendInt("/numberOfClick", 0);
      numberOfClick = 0;
    }
  }*/

  /*if (millis() - monChronoMessageDeux >= 600) {
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
  }*/
  

}