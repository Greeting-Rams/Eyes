/*
Greetings Ram - Blinking Eye Code
Authors: Michael Lu & Jose Mojica-Ramos (Fall 2024)
Editor: Jose Mojica-Ramos
Edited: March 26, 2025
*/


//  Eye Mechanism Code
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 140  // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 520  // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;                 // "Anything with an "x" is Left/Right, Anything with a "y" is Up/Down" - Michael
int xVal, yVal;
int xSum = 512, ySum = 512;
int xMovement;                            // Left/Right Movement
int UpY_Movement, DownY_Movement;         // Up/Down Movement
int TL_Lid, BL_Lid, TR_Lid, BR_Lid;       // Eyelids variables
int trimval;
int tempVar;                              // Temp Value for moving left and right
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

int sensorValue = 0;
int outputValue = 0;
int switchval = 0;

void setup() {
  Serial.begin(3000);     // 1.5 sec delay
  Serial.println("\nStarting Program:\n");    // Just to verify in Monitor that code is running

  pinMode(8, INPUT);   //+x
  pinMode(9, INPUT);   //-x
  pinMode(10, INPUT);  //+y
  pinMode(11, INPUT);  //-y

  //INPUT_PULLUP is more stable than INPUT, but OUTPUT is most stable
  pinMode(7, OUTPUT);  //blinking eyes pin

  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);


  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates


  trimval = map(512, 320, 580, -40, 40);
  // Lid movement
  TL_Lid = map(512, 0, 1023, 400, 280) - (trimval - 40);
  TL_Lid = constrain(TL_Lid, 280, 400);
  TR_Lid = 680 - TL_Lid;
  BL_Lid = map(512, 0, 1023, 400, 280) + (trimval / 2);
  BL_Lid = constrain(BL_Lid, 280, 400);
  BR_Lid = 680 - BL_Lid;

  delay(10);
}



// Loop used to manually test all components
void loop() {
  delay(3000);  // 3 Second delay
  Serial.println("Starting Loop.");

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




// Move eyes Right
delay(500);
  Serial.println("Looking RIGHT.");
  digitalWrite(7, HIGH);  // Set Eyes to move right


  xVal = xSum;
  xMovement = map(yVal, 0, 1023, 280, 380);

  pwm.setPWM(0, 0, xMovement);


  if (digitalRead(7) == HIGH) {          // Look Right
    pwm.setPWM(0, 0, xMovement);      // 280
  }

  else if (digitalRead(7) == LOW) {      // Look Left
    tempVar = xMovement + 100;        // 380
    pwm.setPWM(0, 0, tempVar);
  }


// delay(3000);           // 3 second delay before looking left
  delay(2000);
// Move eyes Left
  Serial.println("Looking LEFT.");
  digitalWrite(7, LOW);  // Set Eyes to move left


  xVal = xSum;
  xMovement = map(yVal, 0, 1023, 280, 380);

  pwm.setPWM(0, 0, xMovement);
  Serial.println(xMovement);


  if (digitalRead(7) == HIGH) {          // Look Right
    pwm.setPWM(0, 0, xMovement);      // 280
  }

  else if (digitalRead(7) == LOW) {      // Look Left
    tempVar = xMovement + 100;          // 380
    pwm.setPWM(0, 0, tempVar);
  }


  // Move eyes Center
// delay(3000);
  delay(2000);
  Serial.println("Looking Straight.");
  digitalWrite(7, HIGH);  // Set Eyes to move up


  xVal = xSum;
  xMovement = map(yVal, 0, 1023, 280, 380);

  pwm.setPWM(0, 0, xMovement);
  xMovement = 330;

  if (digitalRead(7) == HIGH) {          // Look Right
    pwm.setPWM(0, 0, xMovement);      // 280
  }

  else if (digitalRead(7) == LOW) {      // Look Left
    tempVar = xMovement + 100;        // 380
    pwm.setPWM(0, 0, tempVar);
  }









//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$4
// Good Code


  // // First Blink
  // // Setup to CLOSE eyes
  // Serial.println("First Blink.");
  // digitalWrite(7, HIGH);  // Set Eyes to CLOSE


  // xVal = xSum;
  // xMovement = map(xVal, 0, 1023, 280, 380);

  // yVal = ySum;
  // UpY_Movement = map(yVal, 0, 1023, 380, 500);


  // pwm.setPWM(0, 0, xMovement);
  // pwm.setPWM(1, 0, UpY_Movement);


  // if (digitalRead(7) == HIGH) {  // Closes Eyes
  //   pwm.setPWM(2, 0, 605);
  //   pwm.setPWM(3, 0, 135);
  //   pwm.setPWM(4, 0, 95);
  //   pwm.setPWM(5, 0, 555);
  // }

  // else if (digitalRead(7) == LOW) {  // Open Eyes
  //   pwm.setPWM(2, 0, TL_Lid);
  //   pwm.setPWM(3, 0, BL_Lid);
  //   pwm.setPWM(4, 0, TR_Lid);
  //   pwm.setPWM(5, 0, BR_Lid);
  // }

  // // Gets ready to Open eyes
  // delay(200);            // 0.2 Second delay to replicate blinking
  // digitalWrite(7, LOW);  // Sets Eyes to Open


  // xVal = xSum;
  // xMovement = map(xVal, 0, 1023, 280, 380);

  // yVal = ySum;
  // UpY_Movement = map(yVal, 0, 1023, 380, 500);


  // pwm.setPWM(0, 0, xMovement);
  // pwm.setPWM(1, 0, UpY_Movement);


  // if (digitalRead(7) == HIGH) {
  //   pwm.setPWM(2, 0, 605);
  //   pwm.setPWM(3, 0, 135);
  //   pwm.setPWM(4, 0, 95);
  //   pwm.setPWM(5, 0, 555);
  // }

  // else if (digitalRead(7) == LOW) {
  //   pwm.setPWM(2, 0, TL_Lid);
  //   pwm.setPWM(3, 0, BL_Lid);
  //   pwm.setPWM(4, 0, TR_Lid);
  //   pwm.setPWM(5, 0, BR_Lid);
  // }

  // delay(2500);    //2.5 Sec delay 

// End of Good Code
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


  // Serial.println("Finished loop **********************************************");
  Serial.println("Resetting Loop.\n");
  // delay(10000);     // 10 second delay
  // delay(30000);  // 30 sec delay
  delay(2000);
  // delay(300000);  // LONG delay ~5 Min
}
// End of manual testing loop
