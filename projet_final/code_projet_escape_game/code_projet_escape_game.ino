#define CHAN_KEY 0
#define CHAN_ANGLE 1
#define CHAN_KEY_COOL 2

#include <M5Atom.h>

CRGB pixel[1];
//CRGB mesPixels[1];

unsigned long monChronoMessages;
unsigned long monChronoMessageDeux;

int lightsOn;

#define LONGUEUR 30
CRGB maBandeDel[LONGUEUR];

#include <MicroOscSlip.h>
MicroOscSlip<128> monOsc(&Serial);

#include <M5_PbHub.h>
M5_PbHub myPbHub;

#include <VL53L0X.h>
VL53L0X myTOF;


int maLectureKeyPrecedente;
int maLectureKeyPrecedenteCool;
bool etatPlay = false;
bool etatPlayCool;

int numberOfClick;
int red;
int green;


void setup() {
  // put your setup code here, to run once:
  M5.begin(false, false, false);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(pixel, 1);  // Ajouter le pixel du M5Atom à FastLED
  //FastLED.addLeds<WS2812, 26, RGB>(maBandeDel, LONGUEUR);
  Serial.begin(115200);

  unsigned long chronoDepart = millis();
  while (millis() - chronoDepart < 5000) {
    pixel[1] = CRGB(255, 255, 255);
    FastLED.show();
    delay(100);

    pixel[1] = CRGB(0, 0, 0);
    FastLED.show();
    delay(100);
  }

  pixel[1] = CRGB(0, 0, 0);
  FastLED.show();

  Wire.begin();
  myPbHub.begin();
  myPbHub.setPixelCount(CHAN_KEY, 1);

  myTOF.init();
  myTOF.setTimeout(500);
  myTOF.startContinuous();


  // Animation de démarrage
  /*while (millis() < 5000) {
    mesPixels[0] = CHSV((millis() / 5) % 255, 255, 255 - (millis() * 255 / 5000));
    FastLED.show();
    delay(50);
  }
  mesPixels[0] = CRGB(0, 0, 0);
  FastLED.show();*/


   myPbHub.setPixelColor(CHAN_KEY , 0, 255 , 255, 255);

 lightsOn = 0;
  numberOfClick = 0;
  red = 0;
  green = 100;


}


void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  /*int monMin = 0;
  int monMax = 256;
  int monRouge = random( monMin , monMax );
  int monVert = random( monMin , monMax ); 
  int monBleu = random( 0 , 256 );*/
  //myPbHub.setPixelColor(channel , 1, random( 0 , 256 ) , random( 0 , 256 ), random( 0 , 256 ));

  // À CHAQUE 20 MS I.E. 50x PAR SECONDE
  if (millis() - monChronoMessages >= 20) {
    monChronoMessages = millis();

    int maLectureKey = myPbHub.digitalRead(CHAN_KEY);

    if (maLectureKeyPrecedente != maLectureKey) {
      
      
      if (maLectureKey == 0) {
        //myPbHub.setPixelColor( CHAN_KEY, 0, random( 0 , 256 ) , random( 0 , 256 ), random( 0 , 256 ));
        if(etatPlay == false){
          monOsc.sendFloat("/key", 1);
          etatPlay = true;
        }else{
          monOsc.sendFloat("/key", 0);
          etatPlay = false;
        }
        
        /*monOsc.sendFloat("/vkb_midi/@/note/52", 64);
        monOsc.sendFloat("/vkb_midi/@/note/55", 64);*/
      }else{
        //monOsc.sendFloat("/vkb_midi/@/note/48", 0);
        /*monOsc.sendFloat("/vkb_midi/@/note/52", 0);
        monOsc.sendFloat("/vkb_midi/@/note/55", 0);*/
      }
    }
    maLectureKeyPrecedente = maLectureKey;

    int maLectureKeyCool = myPbHub.digitalRead(CHAN_KEY_COOL);

    

    if (maLectureKeyPrecedenteCool != maLectureKeyCool) {
      if (maLectureKeyCool == 0) {
        monOsc.sendInt("/keyCool", maLectureKeyCool);
        etatPlayCool = !etatPlayCool;
        monOsc.sendInt("/numberOfClick", numberOfClick);

        //maBandeDel[numberOfClick] = CRGB(red, green, 0);
        //FastLED.show();

        numberOfClick += 1;
        if (numberOfClick >= random(8, 28)) {
          red = 100;
          green = 0;
        }
        if (numberOfClick == 30) {
          for (int i=0; i < LONGUEUR; i++) {
            //maBandeDel[i] = CRGB(0, 0, 0);
          }
          //FastLED.show();
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
    
    int maLectureAngle = myPbHub.analogRead(CHAN_ANGLE);
    //int tune = maLectureAngle / 4095.0 * 127;
    monOsc.sendInt("/angle", maLectureAngle);

    /*int maLectureLumiere = myPbHub.analogRead(CHAN_LIGHT);
    int mixReverb = maLectureLumiere / 4095.0 * 127 / 2;
    //int mixReverb = map(maLectureLumiere, 1300, 3700, 0, 127);
    if(mixReverb <= 34 ){
      monOsc.sendInt("/vkb_midi/9/cc/12", mixReverb);
    }else{
      monOsc.sendInt("/vkb_midi/9/cc/12", 0);
    }*/
    

    uint16_t millimetres = myTOF.readRangeContinuousMillimeters();
    //int pos =  millimetres/ 500.0 * 127;
    int pos = map(millimetres, 0, 1200, 0, 127);
    int amount = 0;
    if(millimetres <= 1200){
      monOsc.sendInt("/appear", 1);


      //code pour version originale
      /*int randyNumby = random(1,4);
      if(randyNumby == 1){
        myPbHub.setPixelColor(CHAN_KEY , 255, 0, 0, 0);
      }else if(randyNumby == 2){
        myPbHub.setPixelColor(CHAN_KEY , 0, 255, 0, 0);
      }else if(randyNumby == 3){
        myPbHub.setPixelColor(CHAN_KEY , 0, 0 , 255, 0);
      }else{
        myPbHub.setPixelColor(CHAN_KEY , 0, 0 , 0, 255);
      }*/
      //monOsc.sendInt("/vkb_midi/9/cc/13", pos);
      //myPbHub.setPixelColor(channel , CHAN_KEY, 255 , 255, 255);
    }else{
      monOsc.sendInt("/appear", 0);
      monOsc.sendInt("/static", 1);

      //monOsc.sendInt("/vkb_midi/9/cc/13", 0);
    }
    
    

    //monOsc.sendInt("/milimitr", value);


  }
}