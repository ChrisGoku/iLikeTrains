#include "MeMegaPi.h"
#include "BotClass.h"
#include "FastLED.h"
#define NUM_LEDS 2
#define RIGHT_LED A13
#define LEFT_LED A14
//CRGB ledr[NUM_LEDS - 1];
//CRGB ledl[NUM_LEDS -1 ];

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
int top = digitalRead(TOP_IR_SENSOR);
int top_counter = 0;

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
  // if (!top) {
  //   top_counter = top_counter + 1;
  //   //Serial.println(bot.top_motion);
  
 // Serial.println(bot.extraction_button);
  if (bot.extraction_button < 3){
    bot.lineTracking();
   
  }
   

  if (bot.extraction_button == 3 && bot.top_motion == 0) {
    // Serial.println("HORRAY!");
    bot.avoidObstacles();
  }

 

  if (bot.top_counter == 1) {
    bot.signal_exchange();
     // if the bot detects hand wave, switch to signal exchange, also includes rendevous
     //reset, avoidObstacle will start again so pick up, press switch, put on line to return
  } 
  if (bot.top_counter > 1){
    bot.rendevous_mode();
  }

  // THIS IS WHERE WE IMPLEMENT THE IMPACT SWITCH CHANGE FROM OBS AVIODANCE TO RENDEZVOUS
  // if (bot.extraction_button > 3)
  // {
  //   bot.rendezvous_mode();
  // }

  // THIS IS WHERE WE WILL IMPLEMENT IMPACT SWITCH CHANGE FROM RENDEZVOUS MODE TO LINE TRACKING
  // for now it is just from obstacle avoidance to line tracking to made sure I know how to do this
  if (bot.extraction_button > 3)
  {
    bot.lineTracking();
  }

  // bot.print();

}
