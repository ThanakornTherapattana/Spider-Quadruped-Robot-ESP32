#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

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
    writeServos();
    delay(15);
  }


//linear push back
  for(int step=0; step<=step_num; step++){
    float t = (float)step/step_num;
    x = t*step_length + x_start;
    y = Y_still;
    z = Z_still;

    IK_calculation(x, y, z);
    writeServos();
    delay(15);
  }
}

void writeServos() {
  pwm.setPWM(0, 0, angleToPulse(theta1));
  pwm.setPWM(1, 0, angleToPulse(theta2));
  pwm.setPWM(2, 0, angleToPulse(theta3));
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

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

  IK_calculation(X_still, Y_still, Z_still);

  writeServos();

}

void loop() {
  // put your main code here, to run repeatedly:
  if ( Serial.available()){
    float x = Serial.parseFloat();
    float y = Serial.parseFloat();
    float z = Serial.parseFloat();

    IK_calculation(x, y, z);
    writeServos();
  }
  delay(100);
  coord_calculation();
  
}
