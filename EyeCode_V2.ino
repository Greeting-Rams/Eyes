/*
Greetings Ram - Blinking Eye Code
Authors: Michael Lu & Jose Mojica-Ramos (Fall 2024)
Editor: Jose Mojica-Ramos & Michelle Vasquez
Edited: April 2, 2025
*/


//  Eye Mechanism Code
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 140  // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 520  // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;  // "Anything with an "x" is Left/Right, Anything with a "y" is Up/Down" - Michael
int xVal, yVal;
int xSum = 512, ySum = 512;
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

int sensorValue = 0;
int outputValue = 0;
int switchval = 0;

// Serial Motor Input
char receivedChar;
boolean newData = false;

void setup() {
  Serial.begin(9600);                       // Serial Initialization **(DON'T TOUCH)**
  Serial.println("\nStarting Program:\n");  // Just to verify in Monitor that code is running

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
  recInfo();
  loop2();
}




// Takes input to then call the right case
void recInfo() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    newData = true;
  }
}

// void lightLED(){
//   int led = (receivedChar - '0');
//   while(newData == true){
//     digitalWrite(led, HIGH);
//     delay(500);
//     digitalWrite(led, LOW);
//     newData = false;
//   }
// }



// Loop used to manually test all components
void loop2() {
  delay(1000);
  int led = (receivedChar - '0');
  // while (newData == true) {
    Serial.println("Starting Loop.");
    delay(100);
    Serial.print("Received Character: ");
    Serial.println(receivedChar);
    delay(100);

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




    // Looking RIGHT
    if (receivedChar == 'R') {
      digitalWrite(led, HIGH);
      delay(100);
      Serial.println("Looking RIGHT.");
      digitalWrite(7, HIGH);  // Set Eyes to move right


      xVal = xSum;
      xMovement = map(yVal, 0, 1023, 280, 380);

      pwm.setPWM(0, 0, xMovement);


      if (digitalRead(7) == HIGH) {   // Look Right
        pwm.setPWM(0, 0, xMovement);  // 280
      }

      else if (digitalRead(7) == LOW) {  // Look Left
        tempVar = xMovement + 100;       // 380
        pwm.setPWM(0, 0, tempVar);
      }
    }



    // Looking LEFT
    if (receivedChar == 'L') {
      digitalWrite(led, LOW);
      delay(100);
      Serial.println("Looking LEFT.");
      digitalWrite(7, LOW);  // Set Eyes to move left


      xVal = xSum;
      xMovement = map(yVal, 0, 1023, 280, 380);

      pwm.setPWM(0, 0, xMovement);
      Serial.println(xMovement);


      if (digitalRead(7) == HIGH) {   // Look Right
        pwm.setPWM(0, 0, xMovement);  // 280
      }

      else if (digitalRead(7) == LOW) {  // Look Left
        tempVar = xMovement + 100;       // 380
        pwm.setPWM(0, 0, tempVar);
      }
    }




    // Move eyes CENTER
    if (receivedChar == 'C') {
      digitalWrite(led, HIGH);
      delay(100);
      Serial.println("Looking Straight.");
      digitalWrite(7, HIGH);  // Set Eyes to move up


      xVal = xSum;
      xMovement = map(yVal, 0, 1023, 280, 380);

      pwm.setPWM(0, 0, xMovement);
      xMovement = 330;

      if (digitalRead(7) == HIGH) {   // Look Right
        pwm.setPWM(0, 0, xMovement);  // 280
      }

      else if (digitalRead(7) == LOW) {  // Look Left
        tempVar = xMovement + 100;       // 380
        pwm.setPWM(0, 0, tempVar);
      }
    }
  // }

    // Resetting for next loop
    receivedChar = 0;
    newData = false;




  // Serial.println("Finished loop **********************************************");
  Serial.println("Resetting Loop.\n");
  delay(100);  // 2 second delay
}
// End of manual testing loop
