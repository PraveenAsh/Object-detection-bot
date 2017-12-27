#include <AFMotor.h> 
#include <Servo.h>    
#include <NewPing.h>

#define TRIG_PIN A4 
#define ECHO_PIN A5
#define MAX_DISTANCE_POSSIBLE 1000 
#define MAX_SPEED 150 // 
#define MOTORS_CALIBRATION_OFFSET 3
#define COLL_DIST 20 
#define TURN_DIST COLL_DIST+10 
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE_POSSIBLE);

/*Exclusively Praveen C Naik's , code Verson 1.0*/
#define H_TRIG_PIN A2 
#define H_ECHO_PIN A3
#define H_MAX_DIST 1000
NewPing bolar(H_TRIG_PIN,H_ECHO_PIN,H_MAX_DIST);

AF_DCMotor leftMotor(2, MOTOR12_8KHZ); 
AF_DCMotor rightMotor(1, MOTOR12_8KHZ); 
Servo neckControllerServoMotor;

int pos = 0; 
int maxDist = 0;
int maxAngle = 0;
int maxRight = 0;
int maxLeft = 0;
int maxFront = 0;
int course = 0;
int curDist = 0;
String motorSet = "";
int speedSet = 0;
int loopCount = 0;

void setup() {
  /*Exclusively Praveen C Naik's , code Verson 1.0*/
  Serial.begin(9600);
  neckControllerServoMotor.attach(10);  
  neckControllerServoMotor.write(90); 
  delay(2000);
  checkPath(); 
  motorSet = "FORWARD"; 
  neckControllerServoMotor.write(90);
  moveForward();
}



void loop() {
  Serial.print("---------------------------CLOCK CYCLE DONE :");
  Serial.print(loopCount++);
  Serial.println("---------------------------");
  checkForward();  
  checkPath();
}

void checkPath() {
  int curLeft = 0;
  int curFront = 0;
  int curRight = 0;
  int curDist = 0;
  neckControllerServoMotor.write(144);
  delay(120); 
  for(pos = 144; pos >= 36; pos-=18)
  {
        if(deadLock()==0)
    {
          Serial.println("i stopped but nothing happened");
          moveBackward2();//this will turn in a bigger reverse
      setCourse();//this will reset the servo angle to check for new incoming objects
          break;
        }
        else
    {
            neckControllerServoMotor.write(pos);
            delay(90);
            checkForward(); 
            curDist = readPing();
            Serial.print("Ultasonic reading is : ");
            Serial.println(curDist);
            if (curDist < COLL_DIST)
      {
                  checkCourse();
                  Serial.println("curDist less than COLL_DIST");
                  break;
      }
      if (curDist < TURN_DIST) 
      {
        changePath();
      }
      if (curDist > curDist) {maxAngle = pos;}
      if (pos > 90 && curDist > curLeft) { curLeft = curDist;}
      if (pos == 90 && curDist > curFront) {curFront = curDist;}
      if (pos < 90 && curDist > curRight) {curRight = curDist;}
    }
  }
    maxLeft = curLeft;
      maxRight = curRight;
      maxFront = curFront;
}
void setCourse() {
    if (maxAngle < 90) {turnRight();}
    if (maxAngle > 90) {turnLeft();}
    maxLeft = 0;
    maxRight = 0;
    maxFront = 0;
}

void checkCourse() {
  moveBackward();
  delay(500);
  moveStop();
  setCourse();
}

void changePath() {
  if (pos < 90) {lookLeft();} 
  if (pos > 90) {lookRight();}
}


int readPing() {
  delay(70);
  unsigned int uS = sonar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}

/*Exclusively Praveen C Naik's , code Verson 1.0*/
int readHeight() {
  delay(70);
  unsigned int uS = bolar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}

/*Exclusively Praveen C Naik's , code Verson 1.0*/
/*Dedicated functions for motor movement handling*/

void checkForward() { if (motorSet=="FORWARD") {leftMotor.run(FORWARD); rightMotor.run(FORWARD); } }

void checkBackward() { if (motorSet=="BACKWARD") {leftMotor.run(BACKWARD); rightMotor.run(BACKWARD); } }

void moveStop() {leftMotor.run(RELEASE); rightMotor.run(RELEASE);}

void moveForward() {
    motorSet = "FORWARD";
    leftMotor.run(FORWARD);
    rightMotor.run(FORWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2)
  {
    leftMotor.setSpeed(speedSet+MOTORS_CALIBRATION_OFFSET);
    rightMotor.setSpeed(speedSet);
    delay(5);
  }
  Serial.println("JUST MOVED FORWARD");
}

void moveBackward() {
    motorSet = "BACKWARD";
    leftMotor.run(BACKWARD);
    rightMotor.run(BACKWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2)
  {
    leftMotor.setSpeed(speedSet+MOTORS_CALIBRATION_OFFSET);
    rightMotor.setSpeed(speedSet);
    delay(5);
  }
  Serial.println("JUST MOVED BACKWARD");
}  

void moveBackward2() {
    motorSet = "BACKWARD";
    leftMotor.run(BACKWARD);
    rightMotor.run(BACKWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2)
  {
    Serial.println("Right motor works");
    leftMotor.setSpeed(speedSet);//keep changing this value
    rightMotor.setSpeed(speedSet+60);
    delay(5);
  }
}  
void turnRight() {
  motorSet = "RIGHT";
  leftMotor.run(FORWARD);
  rightMotor.run(BACKWARD);
  delay(400);
  motorSet = "FORWARD";
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
}  

void turnLeft() {
  motorSet = "LEFT";
  leftMotor.run(BACKWARD);
  rightMotor.run(FORWARD);
  delay(400);
  motorSet = "FORWARD";
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
}

int deadLock() {
  int responsiveHeight = readHeight();
  Serial.println(responsiveHeight);
  if(responsiveHeight > 4) {
    Serial.println("EDGE DETECTED , I WILL STOP");
    moveStop();
    Serial.println("STOPPED");
    return 0;
  }
  else{
    Serial.println("SURFACE IS SMOOTH/LEVELED");
  }
}
void lookRight() {rightMotor.run(BACKWARD); delay(400); rightMotor.run(FORWARD);}
void lookLeft() {leftMotor.run(BACKWARD); delay(400); leftMotor.run(FORWARD);}
