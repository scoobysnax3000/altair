#include <PinChangeInterrupt.h>

//Define motor driver pins
#define stp 3
#define dir 2
#define MS1 4
#define MS2 5
#define EN  6

//Define Global Vars
int rupt = 2; //defining photogate analog pin
char user_input;
int stepCount = 0; //for debugging and positioning
int x=0;
int direction = 0;

////Interupt functions
//volatile uint8_t triggerState=0; //need this to switch pin commands

void setup() { //this code runs only once, no functions here
Serial.begin(9600);
Serial.println("Begin motor control");
  Serial.println();
  //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. Reverse 4085 steps");
  Serial.println("2. Reverse at 1/8th microstep mode");
  Serial.println("3. Return to calibration point");
  Serial.println();

  //Set pin modes for the motor and interupt pins
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
}

void loop() {
  while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
      if (user_input =='1')
      {
         stepCount = 0;
         reverse();
      }
//      if (user_input =='2')
//      {
//         stepCount = 0;
//         forward();
//      }
      // THIS IS WHAT WE'RE WORKING ON, CALIBRATION
      if (user_input =='2')
      {
         stepCount = 0;
         calibration();
      }
//      else
//      {
//        Serial.println("Invalid option entered.");
//      }
      resetEDPins();
  }
  // put your main code here, to run repeatedly:

}
////////////////////////////////////////////////////////////////////
void reverse(){
  Serial.println("Moving back ");
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
  digitalWrite(MS2, HIGH);
  //stepCount = 0;
//  while(triggerState == 0 && stepCount<3000){  
  for (x= 0; x<200 ; x++){
    direction = 2;
    //analogWrite(stp, 128);
    digitalWrite(stp,HIGH); //Trigger one step forward
    stepCount++;
    Serial.println(x);
    delay(2.5);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(2.5);
  }
}

void calibration(){
  int val;

  while (val > 10){
  val = analogRead(rupt);
  digitalWrite(dir, HIGH); //Pull direction pin high
  direction = 2;
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(2.5);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(2.5);
     Serial.println(val);
  }
 Serial.println("donzo.");
 return;
}

void forward(){
  Serial.println("Moving forward 250 steps at full step mode.");
  digitalWrite(dir, LOW); //
  digitalWrite(MS1, HIGH); //
  digitalWrite(MS2, HIGH);
  stepCount = 0;
  for (x= 0; x< 400; x++){
    direction = 2;
    digitalWrite(stp,HIGH); //Trigger one step forward
    stepCount++;
    Serial.println(stepCount);
    delay(2.5);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(2.5);
  }
  resetEDPins();
//  stepForward();
}

void resetEDPins(){
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, LOW);
}
