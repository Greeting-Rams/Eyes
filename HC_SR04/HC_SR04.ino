/*
 * HC-SR04 example sketch
 * baud rate=9600
 *
 * https://create.arduino.cc/projecthub/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-036380
 *
 * initial code by Isaac100 for the ultrasonic sensor--> just acquiring and printing the distance out (link to code above)
 *
 *
 *
 *modified by Jacob Eldridge-- motion sensing and combining motor code
 *Li-Hong (Michael) Liu for the stepper motor
 *
 *can clean up using functions ex funtion for collect data and function for blinking
 *2 data sets- sums the values and compares the 2 to eachother--> if difference is + going away, if difference is - coming toward
 *
 ***********************************************************************************
 *revision by Jacob Eldridge 4/26/2024
 *added and modified delays for the stepper motor
 *added a delay if a person is already greeted (since using microsecond delays)
 *modified the delta and data to float, since everyting became scaled to microseconds
 *
 *end of revisions by 4/26/2024
 *
 *revisions5/01/2024
 *most fo the counters were getting overflowed (negative)
 *changed from counting up to counting down
 *
 ***************************************************************************************
 *
 */

//use 5V for the Vcc
const int trigPin = 9;//hook trigger pin to 9
const int echoPin = 10;//hook echo pin to 10
const int helloPin=12;//hook pin 12 on this arduino to analog 0 on other
const int goodbyePin=13;//hook pin 13 on this arduino to analog 1 on other

// Define pin connections & motor's steps per revolution
const int directionPin = 2;
const int stepperPin = 3;
const int stepsPerRevolution = 200;//used 200/8 for step counter 200/8=25


//variables for blinking eyes (motor)
int blinkCounter=4000;//used for the 4000 us delay in original code (high and low) in a us (1E-6 seconds) scale
short eyeClosed=0;//true or false so it knows if need to open or close eye (starts opening eye)
//int motorDelay=4000;//used for delay of motor (for speed)
short stepperPinHighorLow=1;//initially high and need it to check to set it
short blinkStep=0;//used for the steps (originally it is x in the for loop)
short needDelay=0;//true false if needs delay fir motor delay of 1 sec between open/close
int Delay1SecCounter=6;//delay for between blinking open and close in a ms scale
int Delay50msCounter=0;
int personDelay=150;// delay for one person walking towards/away so it doesn't keep repeating audio for one person

//variables for ultrasonic sensor
float duration, distance;

int avg1=0;
int avg2=0;
int element;
int timeOut=100;

//avg walking speed is 4.8 km/hr=1.3333 m/s =1.333E-4 cm/us=.00013333 cm/us
//delay of 2000us=2ms--->avg distance covered in 2,000 us is .2667 cm
int delta=60;//need to test set, initially set to .5 referencing the .2667 above
float data[8];

short saidHello=0;//not sure if need this value thought might help later




void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600); //baud rate for serial monitor

  pinMode(helloPin, OUTPUT);
  pinMode(goodbyePin, OUTPUT);
  //initialize values as high
  digitalWrite(helloPin, HIGH);
  digitalWrite(goodbyePin, HIGH);

   // Ｍotor Outputs
  pinMode(stepperPin, OUTPUT);
  pinMode(directionPin, OUTPUT);

  
}


void loop() 
{



  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  

  //start of data processing here
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);

  // Check if someone is within a certain distance
  if (distance<2100 && distance>10) 
  {
    



    if(eyeClosed < 1 && needDelay < 1 )
    {
    // Clockwise open
    digitalWrite(directionPin, HIGH);
    eyeClosed=1;
    }
    else if(needDelay < 1)
    { //CCW closing
      digitalWrite(directionPin, LOW);
      eyeClosed=0;
    }
    
    //do below if dellay of 1 sec achieved.
    

    // Spin motor
    // Opening 
    for(int x = 0; x < stepsPerRevolution / 8 ; x++) 
    {
      
      //do below if delayof1 sec is false
      if(needDelay < 1)
      {
      digitalWrite(stepperPin, HIGH);
      }

      delayMicroseconds(4000);//controls how fast it opens

      if(needDelay < 1)
      {
        digitalWrite(stepperPin, LOW);
      }

      delayMicroseconds(4000);

    }//end for loop about 20 ms perfect delay for data collecting
    Delay1SecCounter=Delay1SecCounter-1;
 

    if(Delay1SecCounter<=0)
    {
      needDelay=0;
      Delay1SecCounter=6;//each for loop completion=20ms*5=1 sec

    }
    else
    {
      needDelay=1;//need 1 sec delay after each for loop.
    }
    if(timeOut <0 || element>5)
    {
      element=0;
      avg1=0;
      avg2=0;
      timeOut=100;
    }

    //start of data processing here

    data[element]=distance;
    //Serial.println(element); for debugging purposes
      
      //below is for at least 2 data points
      if(element>0)
      {

        if( abs(data[element]-data[element-1])<delta )
        {
          //data is good
          element++;
          //if element was 1 now is 2

        }

        if(element==1)
        {
          element=0;//first or second data point is bad and reset
        }
      
      }
      else //else statement for element>0
      {
        element++;//gets here for 0th element (1st data point)
      }

      if (element>5)
      {

        

      //sum data 0-4 (5 points)
      for(int i=0;i<3;i++)
      {
        avg1=avg1+data[i];
      }

      //sum data 5-9 (5 points)
      for(int i=3;i<6;i++)
      {
        avg2=avg2+data[i];
      }



        if( abs(avg1-avg2)< 70)//changed from intital 20
        {
          //do nothing person is stationary
        }

        else if( (avg2-avg1)<0 )// && needDelay==0
        {
          //acoming towards device
          saidHello=1; //maybe use this later?  can use to count people coming in as well.
          //make pin to analog 0 on other arduino low
          digitalWrite(helloPin, LOW);
          delayMicroseconds(10);//make sure registers by the mp3 shield
          digitalWrite(helloPin, HIGH);
         
          
          Serial.println("Hello");
          //delay(1000);//for testing

          
        }

        else if( (avg2-avg1)>0 )
        {
          //say goodbye to anyone leaving by setting analog 1 to low
          
          digitalWrite(goodbyePin, LOW);
          delayMicroseconds(10);
          digitalWrite(goodbyePin, HIGH);
          saidHello=0;
          
          Serial.println("Goodbye");

          
        }


        element=0;
        avg1=0;
        avg2=0;

      }





    
    timeOut--;
   
  }//end if in range
  //some error readings are out of range, so I don't reset the data.
}//end forever loop


