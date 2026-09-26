#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 130
#define SERVOMAX 580
#define SERVO_FREQ 50


int angleToPulse(float ang){
  int pulse = ang*5/2+130;
  pulse = constrain(pulse, SERVOMIN, SERVOMAX);
  Serial.print("Angle: ");Serial.print(ang);
  Serial.print(" pulse: ");Serial.println(pulse);
  return pulse;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("servo test!");

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

}


void loop() { 
  // put your main code here, to run repeatedly:
  if ( Serial.available()){
    float x = Serial.parseFloat();
    float y = Serial.parseFloat();
    float z = Serial.parseFloat();
    pwm.setPWM(0, 0, angleToPulse(x) );
    pwm.setPWM(1, 0, angleToPulse(y) );
    pwm.setPWM(2, 0, angleToPulse(z) );
    }
  
  delay(500);
}
