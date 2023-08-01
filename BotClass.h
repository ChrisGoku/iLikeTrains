#include "MeMegaPi.h"
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

        void turnLeft(int speed) { // Ability to turn x degrees still needs to be implemented.
            fr_motor.run(speed);
            br_motor.run(speed);
            fl_motor.run(speed);
            bl_motor.run(speed);
          }
        void turnRight(int speed) {
            fl_motor.run(-1*speed);
            bl_motor.run(-1*speed);
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
            turnLeft(speed);
         //   Serial.println("Second Branch");
          } else if (left_line_sensor && !right_line_sensor) {
            turnRight(speed);
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


          
          // Serial.println(right);
          // Serial.println(right);
          // serialprintln(top)
          if (!left) {
            turnRight(speed);
          } else if (!right) {
            turnLeft(speed);
          } else if (!left && !right) {
            turnRight(speed);
          } else {
            moveForward(speed);
          }
        }

        void print(){
          int left = digitalRead(LEFT_IR_SENSOR);
          int right = digitalRead(RIGHT_IR_SENSOR);
          int top = digitalRead(TOP_IR_SENSOR); 

          Serial.print(left      );
          Serial.print(top       );
          Serial.print(right     );
          Serial.println();
          delay(500);
        }

        
  };
