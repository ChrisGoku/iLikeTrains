#include "MeMegaPi.h"
#include "BotClass.h"
#include "FastLED.h"
#define NUM_LEDS 2
#define RIGHT_LED A13
#define LEFT_LED A14
CRGB ledr[NUM_LEDS - 1];
CRGB ledl[NUM_LEDS -1 ];

// All pins, double check that leds are correct, I (Daniel) couldn't turn them on when I tried
int  LEFT_IR_SENSOR = A6; 
int  TOP_IR_SENSOR = A7;
int  RIGHT_IR_SENSOR = A8;
int  LEFT_IMPACT_SWITCH = A11;
int  RIGHT_IMPACT_SWITCH = A12;
// int  RIGHT_LED = A14;
// int  LEFT_LED = A13;
int  RIGHT_LINE_TRACKER = A10;
int  LEFT_LINE_TRACKER = A9;

MeMegaPiDCMotor FR_MOTOR(PORT1A);

MeMegaPiDCMotor BR_MOTOR(PORT1B);

MeMegaPiDCMotor FL_MOTOR(PORT2A);

MeMegaPiDCMotor BL_MOTOR(PORT2B);



Bot bot; // Initialize our bot

void setup() {
  // put your setup code here, to run once:
  bot.init(FR_MOTOR, FL_MOTOR, BR_MOTOR, BL_MOTOR);
  FastLED.addLeds<WS2812B, RIGHT_LED, RGB>(ledr, NUM_LEDS - 1);
  FastLED.addLeds<WS2812B, LEFT_LED, RGB>(ledl, NUM_LEDS - 1);
  
  Serial.begin(9600);

}

void loop() {
  ledr[0] = CRGB::White; 
	ledl[0] = CRGB::White; 
  FastLED.show(); 
  // bot.avoidObstacles();
  bot.update();
  
  bot.lineTracking();
  if (bot.extraction_button > 2) {
    // Serial.println("HORRAY!");
    bot.avoidObstacles();
  }

  // bot.print();

}
