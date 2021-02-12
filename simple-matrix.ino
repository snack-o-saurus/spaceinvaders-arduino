#include <FastLED.h>
#include "Matrix.h"

#define LED_PIN     2
#define BUTTON_PIN  3
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define BRIGHTNESS  200

// Player objects
Paddle paddle(1, 1, 5, 1, CRGB(255, 0, 255), false);
Ball ball(5, 6, 1, 1, CRGB(0, 0, 255), 500);

// Refresh & timer object
Surface surface(50);

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(UncorrectedColor);
  FastLED.setBrightness( BRIGHTNESS );
  
  ball.attachCollider(&paddle);
  ball.attachSurface(&surface);

  paddle.draw();
}


void loop()
{
  if(Serial > 0)
{
  int scorelist[128];
  // -> highscore

  surface.timer();

  paddle.move(map(analogRead(A0), 0, 1023, 0, kMatrixWidth - paddle.getWidth()), kMatrixHeight - 4);
  ball.move();

  surface.refresh();
  }
}

//------------------------------------------------------------------------

String menu(String playername)
{
  String pname;

  Serial.println("######SPACE INVADERS#####");
  Serial.println("          MENU           ");
  Serial.println("1. SELECT YOUR NAME:     ");
  pname = Serial.readStringUntil("\n");
  Serial.println("YOU HAVE CHOSEN: " + pname);

  return pname;
}


int highscore(int scoring)
{
  // -> from level collider


}
