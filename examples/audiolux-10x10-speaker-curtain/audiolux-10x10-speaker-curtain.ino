#include <pixl.h>
#include <FastLED.h>
#include <Logging.h>

// Weird thing I had to do to get the Logging library working with teensy
extern "C"{
  int _getpid(){ return -1;}
  int _kill(int pid, int sig){ return -1; }
  int _write(){return -1;}
}

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixl;

Input* input;

LEDStrip strip = LEDStrip(100);
LEDs leds1 = LEDs(&strip, 0, 100);
//LEDs leds2 = LEDs(&strip, 1, 101);

Visualization* viz;
CurtainAnimation* anim1;
//CurtainAnimation* anim2;

void setup() {
  Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Info("Starting setup()\n");
  Serial.flush();
  delay(1000);

  //input = new NoteInput();
  input = new MSGEQ7Input(
    1,    // left pin
    0,    // right pin
    12,   // strobe pin
    13,   // reset pin,
    1000, // max amplitude
    250   // min amplutide
  );

  viz = new RippleVisualization(input, 25, 1, true);

  anim1 = new CurtainAnimation(viz, leds1);
  //anim2 = new CurtainAnimation(viz, leds2);
  delay(100);

  anim1->init(
     10,    // height in pixels
     10,     // width in pixels
     1.0,   // height in ratio of visualiation
     1.0,  // width in ratio of visualiation
     1.57,   // rotation in radians
     0.0,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 100);
  FastLED.show();

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim1);
//  looper->addAnimation(anim2);
  looper->setUpdatesPerSecond(30);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  Looper::instance()->loop();
}
