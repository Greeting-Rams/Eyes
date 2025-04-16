/*
Greetings Ram - Blinking Eye Code
          This version takes letters from Rasbperry Pi to control eyes
Authors: Michael Lu & Jose Mojica-Ramos (Fall 2024)
Editor: Jose Mojica-Ramos & Michelle Vasquez
Edited: April 9, 2025
*/


#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo Constants
#define SERVOMIN 140  // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 520  // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;  // "Anything with an "x" is Left/Right, Anything with a "y" is Up/Down" - Michael
int xVal, yVal;
int xSum = 512, ySum = 512;     // Eye position Variables
int xMovement;                       // Left/Right Movement
int UpY_Movement, DownY_Movement;    // Up/Down Movement
int TL_Lid, BL_Lid, TR_Lid, BR_Lid;  // Eyelids variables
int trimval;
int tempVar;  // Temp Value for moving left and right
/* Legend:
      LR    : Left-Right        Pin 0     (280 = Right, 380 = Left)
      EUD   : Eyes Up-Down      Pin 1     (380, 500)
      TL_Lid: Top-Left Lid      Pin 2     605
      BL_Lid: Bottom-Left Lid   Pin 3     135
      TR_Lid: Top-Right Lid     Pin 4     95
      BR_Lid: Bottom-Right Lid  Pin 5     555
      Gold-Arm: Arm Servo 1     Pin 6     45
      Black_Arm: Arm Servo 2    Pin 7     180
*/

// Serial Motor Input
char receivedChar;
boolean newData = false;

void setup() {
  Serial.begin(9600);                       // Serial Initialization **(DON'T TOUCH)**
  Serial.println("\nStarting Program:\n");  // Just to verify in Monitor that code is running

  // Input Pin setup
  pinMode(8, INPUT);   //+x
  pinMode(9, INPUT);   //-x
  pinMode(10, INPUT);  //+y
  pinMode(11, INPUT);  //-y

  // Output pin setup
  pinMode(7, OUTPUT);

  // Initial pin state
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);

  // PWM Setup
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates


  // Determine initial eyelid position
  trimval = map(512, 320, 580, -40, 40);
  TL_Lid = constrain(map(512, 0, 1023, 400, 280) - (trimval - 40), 280, 400);
  TR_Lid = 680 - TL_Lid;
  BL_Lid = constrain(map(512, 0, 1023, 400, 280) + (trimval / 2), 280, 400);
  BR_Lid = 680 - BL_Lid;

  delay(10);
}

void loop() {
  recInfo();
  EyeMovement();
}

// Takes input to then call the right case
void recInfo() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    newData = true;
  }
}


// Function that uses serial input to control the eye movement
void EyeMovement() {
  delay(1000);
  int tempLetter = (receivedChar - '0');
  Serial.println("Starting Loop.");
  delay(100);
  Serial.print("Received Character: ");
  Serial.println(receivedChar);
  delay(100);

  xSum = 512;   // Resetting Eye Position
  ySum = 512;

  // Adjust eye position based on digital input
  if (digitalRead(8) == HIGH && xSum < 1024) {
    xSum += 20;
  }
  if (digitalRead(9) == HIGH && xSum > 0) {
    xSum -= 20;
  }
  if (digitalRead(10) == HIGH && ySum < 700) {
    ySum += 20;
  }
  if (digitalRead(11) == HIGH && ySum > 0) {
    ySum -= 20;
  }

  // Calls function to determine eye movement
  moveEyes(receivedChar, tempLetter);

  // Resetting for next loop
  receivedChar = 0;
  newData = false;

  Serial.println("Resetting Loop.\n");
  delay(100);  // 0.1 second delay
}

void moveEyes(char direction, int tempLetter){
  int  xMovement = map(ySum, 0, 1023, 280, 380);

  // Look RIGHT
  if(direction == 'R'){
    digitalWrite(tempLetter, HIGH);
    delay(100);
    Serial.println("Looking RIGHT.");
    digitalWrite(7, HIGH);              // Set Eyes to move right

    pwm.setPWM(0, 0, xMovement);

    if (digitalRead(7) == HIGH) {   // Look Right
      pwm.setPWM(0, 0, xMovement);  // 280
    }
    else{                           // Look Left
      tempVar = xMovement + 100;    // 380
      pwm.setPWM(0, 0, tempVar);
    }
  }

  // Looking LEFT
  else if (direction == 'L') {
    digitalWrite(tempLetter, LOW);
    delay(100);
    Serial.println("Looking LEFT.");
    digitalWrite(7, LOW);  // Set Eyes to move left

    pwm.setPWM(0, 0, xMovement);

    if (digitalRead(7) == HIGH) {   // Look Right
      pwm.setPWM(0, 0, xMovement);  // 280
    }
    else {                          // Look Left
      tempVar = xMovement + 100;    // 380
      pwm.setPWM(0, 0, tempVar);
    }
  }

  // Move eyes CENTER
  else if (direction == 'C') {
    digitalWrite(tempLetter, HIGH);
    delay(100);
    Serial.println("Looking Straight.");
    digitalWrite(7, HIGH);  // Set Eyes to move up

    xMovement = 330;
    pwm.setPWM(0, 0, xMovement);
    xMovement = 330;

    if (digitalRead(7) == HIGH) {   // Look Right
      pwm.setPWM(0, 0, xMovement);  // 280
    }

    else {                          // Look Left
      tempVar = xMovement + 100;    // 380
      pwm.setPWM(0, 0, tempVar);
    }
  }
}
