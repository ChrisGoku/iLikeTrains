#include "MeMegaPi.h"
#include "FastLED.h"
#define NUM_LEDS 2
#define RIGHT_LED A13
#define LEFT_LED A14
CRGB ledr[NUM_LEDS - 1];
CRGB ledl[NUM_LEDS -1 ];
//FastLED.addLeds<WS2812B, RIGHT_LED, RGB>(ledr, NUM_LEDS - 1);
//FastLED.addLeds<WS2812B, LEFT_LED, RGB>(ledl, NUM_LEDS - 1);
class Bot { // Class to make accessing motors easier
      public: 
        MeMegaPiDCMotor fr_motor; // Directions are from the perspecitve of looking at the robot from the back
        MeMegaPiDCMotor fl_motor;
        MeMegaPiDCMotor br_motor;
        MeMegaPiDCMotor bl_motor;
        int  LEFT_IR_SENSOR;  
        int  TOP_IR_SENSOR; 
        int  RIGHT_IR_SENSOR; 
        int  LEFT_IMPACT_SWITCH; 
        int  RIGHT_IMPACT_SWITCH;
        int  RIGHT_LED; 
        int  LEFT_LED; 
        int  RIGHT_LINE_TRACKER; 
        int  LEFT_LINE_TRACKER; 
        int right_line_sensor;
        int left_line_sensor;
        int extraction_button;
        int prev;
        int starttime = 0;
        int endtime = 0;
        int top_motion=0;
        int right_motion = 0;
        int right_motion_counter = 0;
        int left_motion = 0;
        int left_motion_counter = 0;
        int input_counter = 0;
        int top_counter = 0;
        
        
        
        Bot() {}
        void init(MeMegaPiDCMotor fr_motor1, MeMegaPiDCMotor fl_motor1, MeMegaPiDCMotor br_motor1, MeMegaPiDCMotor bl_motor1) { // Constructor
          fr_motor = fr_motor1;
          fl_motor = fl_motor1;
          br_motor = br_motor1;
          bl_motor = bl_motor1;
          LEFT_IR_SENSOR = A6; 
          TOP_IR_SENSOR = A7;
          RIGHT_IR_SENSOR = A8;
          LEFT_IMPACT_SWITCH = A11;
          RIGHT_IMPACT_SWITCH = A12;
          RIGHT_LED = A14;
          LEFT_LED = A13;
          RIGHT_LINE_TRACKER = A10;
          LEFT_LINE_TRACKER = A9;
          extraction_button = 0;
          prev = 0;
          
          

          pinMode(LEFT_IR_SENSOR, INPUT);
          pinMode(RIGHT_IR_SENSOR, INPUT);
          pinMode(TOP_IR_SENSOR, INPUT);
          pinMode(LEFT_IMPACT_SWITCH, INPUT);
          pinMode(RIGHT_IMPACT_SWITCH, INPUT);
          // pinMode(RIGHT_LED, OUTPUT);
          // pinMode(LEFT_LED, OUTPUT);
          pinMode(RIGHT_LINE_TRACKER, INPUT);
          pinMode(LEFT_LINE_TRACKER, INPUT);
        }

        void moveForward(int speed) { // Movement member functions
            fr_motor.run(speed);
            fl_motor.run(-1*speed);
            br_motor.run(speed);
            bl_motor.run(-1*speed);
          }
        void moveBackward(int speed) {
            fr_motor.run(-1*speed);
            fl_motor.run(speed);
            br_motor.run(-1*speed);
            bl_motor.run(speed);
          }

        void turnLeft(int speed) {
            fr_motor.run(speed);
            br_motor.run(speed);
            fl_motor.run(speed);
            bl_motor.run(speed);
          }
        void turnRight(int speed) {
            fl_motor.run(-1 * speed);
            bl_motor.run(-1 * speed);
            fr_motor.run(-1 * speed);
            br_motor.run(-1 * speed);
          }

        void allStop() {
            fr_motor.stop();
            fl_motor.stop();
            br_motor.stop();
            bl_motor.stop();
          }
        void lineTracking() { // To be ran in a loop multiple times
          right_line_sensor = digitalRead(RIGHT_LINE_TRACKER);
          left_line_sensor = digitalRead(LEFT_LINE_TRACKER);
          int speed = 60;
          if (right_line_sensor && left_line_sensor) {
            moveForward(speed);
          //  Serial.println("First Branch");
          } else if (right_line_sensor && !left_line_sensor) {
            starttime = millis();
            endtime = starttime;
            while ((endtime - starttime) <= 300) // do this loop for up to 1000mS
            {
              turnLeft(speed);
              endtime = millis();
            }
         //   Serial.println("Second Branch");
          } else if (left_line_sensor && !right_line_sensor) {
            starttime = millis();
            endtime = starttime;
            while ((endtime - starttime) <= 300) // do this loop for up to 1000mS
            {
              turnRight(speed);
              endtime = millis();
            }
          //  Serial.println("Third Branch");
          } else {
            moveForward(speed);
          //  Serial.println("Fourth Branch");
          }

        }
        void update() {
          if (!digitalRead(LEFT_IMPACT_SWITCH) + !digitalRead(RIGHT_IMPACT_SWITCH) != prev) {
            extraction_button += !digitalRead(LEFT_IMPACT_SWITCH) + !digitalRead(RIGHT_IMPACT_SWITCH);
          } 
          prev = !digitalRead(LEFT_IMPACT_SWITCH) + !digitalRead(RIGHT_IMPACT_SWITCH);
          
        }

        void avoidObstacles() {
          int left = digitalRead(LEFT_IR_SENSOR);
          int right = digitalRead(RIGHT_IR_SENSOR);
          int top = digitalRead(TOP_IR_SENSOR);
          int speed = 60;
          
          if (!left) {
            starttime = millis();
            endtime = starttime;
            while ((endtime - starttime) <=300) // do this loop for up to 1000mS
            {
              turnRight(speed);
              endtime = millis();
            }
          } else if (!right) {
            starttime = millis();
            endtime = starttime;
            while ((endtime - starttime) <=350) // do this loop for up to 1000mS
            {
              turnLeft(speed);
              endtime = millis();
            }
          } else if (!left && !right) {
            moveBackward(speed);
            starttime = millis();
            endtime = starttime;
            while ((endtime - starttime) <=350) // do this loop for up to 1000mS
            {
              turnRight(speed);
              endtime = millis();
            }
          } else {
            moveForward(speed);
          }
          if (!top) { // means there IS something there (hand wave)
           top_motion = top_motion + 1;
                   
          } 
         if (top_motion > 0) {
           top_counter = top_counter +1;
            
            
           }
        }
        void rendevous_mode() {
          if (!right) { //counter for different direction hand signals
            right_motion = right_motion + 1;

            if (right_motion > 0)
            {
              right_motion_counter = right_motion_counter + 1;
              input_counter = input_counter + 1;
            }
            //Serial.println(input);
          }
          if (!left) {
            left_motion = left_motion + 1;
            if (left_motion > 0)
            {
              left_motion_counter = left_motion_counter + 1;
              input_counter = input_counter + 1;
            }

          }
          if (input_counter == 4) {
            if (right_motion_counter == 3 && left_motion_counter == 1) { // this will be the correct hand signal (RRRL)
              ledl[0] = CRGB::Green;
              ledr[0] = CRGB::Green;
              FastLED.show();
              delay(2000); //one long flash to signal successful code
              

              right_motion = 0;
              left_motion = 0;
              // top_motion = 0;
              top_counter = 0;
            }
            else {
              //digitalWrite(LEFT_LED, HIGH);
              ledr[0] = CRGB::Red;
              ledl[0] = CRGB::Red;
              FastLED.show();
              delay(2000);
              ledr[0] = CRGB::Black;
              ledl[0] = CRGB::Black;
              FastLED.show();
              //digitalWrite(LEFT_LED, LOW); // signal incorrect
              
              delay(3000);
              //digitalWrite(RIGHT_LED, HIGH);
             ledr[0] = CRGB::Blue;
              ledl[0] = CRGB::Blue;
             FastLED.show();
              delay(500);
             ledr[0] = CRGB::Green;
             ledl[0] = CRGB::Green;
             FastLED.show();
            
             delay(500);
             ledr[0] = CRGB::Black;
             ledl[0] = CRGB::Black;
             FastLED.show();
           
              right_motion = 0;
              left_motion = 0;
              
            }
          }
        }


        void signal_exchange() {
          allStop();
          delay(3000); //show that the robot is the correct robot
          ledr[0] = CRGB::Red;
          ledl[0] = CRGB::Red;
          FastLED.show();
           // digitalWrite(RIGHT_LED, HIGH);
           delay(500);
           ledr[0] = CRGB::Blue;
           ledl[0] = CRGB::Blue;
           FastLED.show();
           // digitalWrite(RIGHT_LED, LOW);
           // digitalWrite(LEFT_LED, HIGH);
            delay(500);
            ledr[0] = CRGB::Black;
            ledl[0] = CRGB::Black;
            FastLED.show();
           // digitalWrite(LEFT_LED, LOW);

            // led flash to signal hand signal read mode
            delay(3000);
            //digitalWrite(RIGHT_LED, HIGH);
            ledr[0] = CRGB::Blue;
            ledl[0] = CRGB::Blue;
            FastLED.show();
            delay(500);
            ledr[0] = CRGB::Green;
            ledl[0] = CRGB::Green;
            FastLED.show();
            
            delay(500);
            ledr[0] = CRGB::Black;
            ledl[0] = CRGB::Black;
            FastLED.show();
           // digitalWrite(RIGHT_LED, LOW);

            right_motion = 0;
            left_motion = 0;
            top_counter = 2;
            //top_counter = top_counter + 1;
            
          
        }

        


        
          int left = digitalRead(LEFT_IR_SENSOR);
        int right = digitalRead(RIGHT_IR_SENSOR);
        int top = digitalRead(TOP_IR_SENSOR);


        void print(){
          left = digitalRead(LEFT_IR_SENSOR);
          right = digitalRead(RIGHT_IR_SENSOR);
          top = digitalRead(TOP_IR_SENSOR); 

          Serial.print(left      );
          Serial.print(top       );
          Serial.print(right     );
          Serial.println();
          delay(500);
        }

        
  };