/*
Greetings Ram - Blinking Eye Code
Authors: Michael Lu & Jose Mojica-Ramos (Fall 2024)
Editor: Jose Mojica-Ramos
Edited: March 5, 2025
*/



//  Eye Mechanism Code
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 140  // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 520  // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;
int xVal, yVal;
int xSum = 512, ySum = 512;
int lexpulse, rexpulse;
int leypulse, reypulse;
int TL_Lid, BL_Lid, TR_Lid, BR_Lid;
int trimval;
/* Legend:
      TL_Lid: Top-Left Lid
      BL_Lid: Bottom-Left Lid
      TR_Lid: Top-Right Lid
      BR_Lid: Bottom-Right Lid
*/


int sensorValue = 0;
int outputValue = 0;
int switchval = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("\nStarting Program:\n");

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



  // Setup to CLOSE eyes
  Serial.println("First Blink.");
  digitalWrite(7, HIGH);  // Set Eyes to CLOSE


  // Serial.print("blinking: ");
  // Serial.println(int(digitalRead(7)));
  // Serial.print("y movement: ");
  // Serial.println(ySum);

  xVal = xSum;
  lexpulse = map(xVal, 0, 1023, 280, 380);

  yVal = ySum;
  leypulse = map(yVal, 0, 1023, 380, 500);


  pwm.setPWM(0, 0, lexpulse);
  pwm.setPWM(1, 0, leypulse);


  if (digitalRead(7) == HIGH) {  // Closes Eyes
    pwm.setPWM(2, 0, 605);
    pwm.setPWM(3, 0, 135);
    pwm.setPWM(4, 0, 95);
    pwm.setPWM(5, 0, 555);
  }

  else if (digitalRead(7) == LOW) {  // Open Eyes
    pwm.setPWM(2, 0, TL_Lid);
    pwm.setPWM(3, 0, BL_Lid);
    pwm.setPWM(4, 0, TR_Lid);
    pwm.setPWM(5, 0, BR_Lid);
  }


  // Gets ready to Open eyes
  delay(200);            // 0.2 Second delay to replicate blinking
  digitalWrite(7, LOW);  // Sets Eyes to Open


  xVal = xSum;
  lexpulse = map(xVal, 0, 1023, 280, 380);

  yVal = ySum;
  leypulse = map(yVal, 0, 1023, 380, 500);


  pwm.setPWM(0, 0, lexpulse);
  pwm.setPWM(1, 0, leypulse);


  if (digitalRead(7) == HIGH) {
    pwm.setPWM(2, 0, 605);
    pwm.setPWM(3, 0, 135);
    pwm.setPWM(4, 0, 95);
    pwm.setPWM(5, 0, 555);
  }

  else if (digitalRead(7) == LOW) {
    pwm.setPWM(2, 0, TL_Lid);
    pwm.setPWM(3, 0, BL_Lid);
    pwm.setPWM(4, 0, TR_Lid);
    pwm.setPWM(5, 0, BR_Lid);
  }

  delay(2500);    //2.5 Sec delay 








// Second Blink

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



  // Setup to Open eyes
  Serial.println("Second Blink.");
  digitalWrite(7, HIGH);  // Set Eyes to Close


  // Serial.print("blinking: ");
  // Serial.println(int(digitalRead(7)));
  // Serial.print("y movement: ");
  // Serial.println(ySum);

  xVal = xSum;
  lexpulse = map(xVal, 0, 1023, 280, 380);

  yVal = ySum;
  leypulse = map(yVal, 0, 1023, 380, 500);


  pwm.setPWM(0, 0, lexpulse);
  pwm.setPWM(1, 0, leypulse);


  if (digitalRead(7) == HIGH) {  // Closes Eyes
    pwm.setPWM(2, 0, 605);
    pwm.setPWM(3, 0, 135);
    pwm.setPWM(4, 0, 95);
    pwm.setPWM(5, 0, 555);
  }

  else if (digitalRead(7) == LOW) {  // Open Eyes
    pwm.setPWM(2, 0, TL_Lid);
    pwm.setPWM(3, 0, BL_Lid);
    pwm.setPWM(4, 0, TR_Lid);
    pwm.setPWM(5, 0, BR_Lid);
  }


  // Gets ready to OPEN eyes
  delay(200);            // 0.2 Second delay to replicate blinking
  digitalWrite(7, LOW);  // Sets Eyes to OPEN


  xVal = xSum;
  lexpulse = map(xVal, 0, 1023, 280, 380);

  yVal = ySum;
  leypulse = map(yVal, 0, 1023, 380, 500);


  pwm.setPWM(0, 0, lexpulse);
  pwm.setPWM(1, 0, leypulse);


  if (digitalRead(7) == HIGH) {
    pwm.setPWM(2, 0, 605);
    pwm.setPWM(3, 0, 135);
    pwm.setPWM(4, 0, 95);
    pwm.setPWM(5, 0, 555);
  }

  else if (digitalRead(7) == LOW) {
    pwm.setPWM(2, 0, TL_Lid);
    pwm.setPWM(3, 0, BL_Lid);
    pwm.setPWM(4, 0, TR_Lid);
    pwm.setPWM(5, 0, BR_Lid);
  }

  delay(2500);










// Winking

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



  // Setup to Wink with Right Eye
  Serial.println("*Wink*");
  digitalWrite(7, HIGH);  // Set Right Eye to CLOSE


  // Serial.print("blinking: ");
  // Serial.println(int(digitalRead(7)));
  // Serial.print("y movement: ");
  // Serial.println(ySum);

  xVal = xSum;
  lexpulse = map(xVal, 0, 1023, 280, 380);

  yVal = ySum;
  leypulse = map(yVal, 0, 1023, 380, 500);


  pwm.setPWM(0, 0, lexpulse);
  pwm.setPWM(1, 0, leypulse);


  if (digitalRead(7) == HIGH) {  // Closes Eyes
    // pwm.setPWM(2, 0, 605);
    // pwm.setPWM(3, 0, 135);
    pwm.setPWM(4, 0, 95);
    pwm.setPWM(5, 0, 555);
  }

  else if (digitalRead(7) == LOW) {  // Open Eyes
    // pwm.setPWM(2, 0, TL_Lid);
    // pwm.setPWM(3, 0, BL_Lid);
    pwm.setPWM(4, 0, TR_Lid);
    pwm.setPWM(5, 0, BR_Lid);
  }


  // Gets ready to OPEN eyes
  // Serial.println("Getting Ready to Blink Quickly.");
  delay(200);            // 0.2 Second delay to replicate blinking
  digitalWrite(7, LOW);  // Sets Eyes to OPEN


  xVal = xSum;
  lexpulse = map(xVal, 0, 1023, 280, 380);

  yVal = ySum;
  leypulse = map(yVal, 0, 1023, 380, 500);


  pwm.setPWM(0, 0, lexpulse);
  pwm.setPWM(1, 0, leypulse);


  if (digitalRead(7) == HIGH) {
    pwm.setPWM(2, 0, 605);
    pwm.setPWM(3, 0, 135);
    pwm.setPWM(4, 0, 95);
    pwm.setPWM(5, 0, 555);
  }

  else if (digitalRead(7) == LOW) {
    // pwm.setPWM(2, 0, TL_Lid);
    // pwm.setPWM(3, 0, BL_Lid);
    pwm.setPWM(4, 0, TR_Lid);
    pwm.setPWM(5, 0, BR_Lid);
  }



  // Serial.println("Finished loop **********************************************");
  Serial.println("Resetting Loop.\n");
  // delay(10000);     // 10 second delay
  // delay(30000);  // 30 sec delay
    delay(300000);  // LONG delay
}