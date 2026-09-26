#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

// servo setup
#define SERVOMIN 130
#define SERVOMAX 580
#define SERVO_FREQ 50


// Link lengths
const float coxa = 41.5;
const float femur = 95.0;
const float tibia = 150.0;

// Link Angles
float theta1, theta2, theta3;


// Others Mathematical Variable
//const float PI = 3.1415;
const float tibia_offset = 14;
const int step_num = 40;
const float step_length = 100.0;
const float step_height = 30.0;

// foot home and still coordinate
float x,y,z;
float X_home = 0;
float Y_home = 115;
float Z_home = -35;
float X_still = 0;
float Y_still = 170;
float Z_still = -50;

void coord_calculation(){
  float x_start = -step_length/2;
  float x_end = step_length/2;

  float rx = step_length/(2*PI);
  float rz = step_height/2;

//cycloidal lift
  for(int step=0; step<=step_num; step++){
    float t = (float)step/step_num;
    x = x_end - rx*(2*PI*t - sin(2*PI*t));
    y = Y_still;
    z = rz*(1.0 - cos(2*PI*t)) + Z_still;

    IK_calculation(x, y, z);
    writeFR(theta1, theta2, theta3);
    delay(15);
  }

//linear push back
  for(int step=0; step<=step_num; step++){
    float t = (float)step/step_num;
    x = t*step_length + x_start;
    y = Y_still;
    z = Z_still;

    IK_calculation(x, y, z);
    writeFR(theta1, theta2, theta3);
    delay(15);
  }
}

void updown_move(){
  //down
  for(int z = Z_still; z>=-100; z--){
    IK_calculation(X_still, Y_still, z);
    writeFR(theta1, theta2, theta3);
    writeBR(theta1, theta2, theta3);
    writeFL(theta1, theta2, theta3);
    writeBL(theta1, theta2, theta3);
    delay(15);
  }
  delay(1000);
  //up
  for(int z = -100; z<=-50; z++){
    IK_calculation(X_still, Y_still, z);
    writeFR(theta1, theta2, theta3);
    writeBR(theta1, theta2, theta3);
    writeFL(theta1, theta2, theta3);
    writeBL(theta1, theta2, theta3);
    delay(15);
  }
  delay(1000);
}

void writeFR(int FR1, int FR2, int FR3) {
  pwm1.setPWM(13, 0, angleToPulse(FR1));
  pwm1.setPWM(14, 0, angleToPulse(FR2));
  pwm1.setPWM(15, 0, angleToPulse(FR3));
}
void writeFL(int FL1, int FL2, int FL3) {
  pwm2.setPWM(0, 0, angleToPulse(FL1));
  pwm2.setPWM(1, 0, angleToPulse(FL2));
  pwm2.setPWM(2, 0, angleToPulse(FL3));
}
void writeBR(int BR1, int BR2, int BR3) {
  pwm1.setPWM(0, 0, angleToPulse(BR1));
  pwm1.setPWM(1, 0, angleToPulse(BR2));
  pwm1.setPWM(2, 0, angleToPulse(BR3));
}
void writeBL(int BL1, int BL2, int BL3) {
  pwm2.setPWM(13, 0, angleToPulse(BL1));
  pwm2.setPWM(14, 0, angleToPulse(BL2));
  pwm2.setPWM(15, 0, angleToPulse(BL3));
}

int angleToPulse(float ang){
  int pulse = ang*2.5+130;
  pulse = constrain(pulse, SERVOMIN, SERVOMAX);
  return pulse;
}

void IK_calculation(float x, float y, float z){
  float b = sqrt(x*x+y*y);
  float a = b-coxa;
  float r = sqrt(z*z+a*a);
  float theta2_cos = (r*r + femur*femur - tibia*tibia) / (2*femur*r);
  float theta3_cos = (femur*femur + tibia*tibia - r*r) / (2*femur*tibia);

  theta1 = atan2(y, x)*180.0/PI;
  theta2 = ( acos(constrain(theta2_cos, -1.0, 1.0)) + atan2(a, -z) )*180.0/PI;
  theta3 = 180.0 - tibia_offset - acos(constrain(theta3_cos, -1.0, 1.0))*180.0/PI;

  theta1 = constrain(theta1, 0, 180);
  theta2 = constrain(theta2, 0, 180);
  theta3 = constrain(theta3, 0, 135);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Inverse Kinematic TEST!");

  pwm1.begin();
  pwm1.setPWMFreq(SERVO_FREQ);
  pwm2.begin();
  pwm2.setPWMFreq(SERVO_FREQ);

  IK_calculation(X_still, Y_still, Z_still);
  writeFR(theta1, theta2, theta3);
  delay(3000);


}

void loop() {
  // put your main code here, to run repeatedly:
  if ( Serial.available()){
    float x = Serial.parseFloat();
    float y = Serial.parseFloat();
    float z = Serial.parseFloat();

    IK_calculation(x, y, z);
    writeFR(theta1, theta2, theta3);
  }
  updown_move();

  
}
