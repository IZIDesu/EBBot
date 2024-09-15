#include <Servo.h>
/* --IZI explanation about Servo library--
  * attach(pin): Binds a servo to a specific pin on the Arduino.
  * detach(): Unbinds the servo from the pin.
  * write(angle): Sets the servo to a specified angle (typically from 0 to 180 degrees).
  * read(): Returns the current angle of the servo.
  * writeMicroseconds(value): Sets the servo position using pulse width modulation (PWM) in microseconds.
  
  / how to apply it?| | |
  -                 V V V
  * Servo ServoName;: Creates a Servo object named myServo.
  * myServo.attach(pin);: Attaches the servo to pin 9 on the Arduino.
  * myServo.write(alpha);: Moves the servo to the 90-degree position.
*/

Servo leftLeg[4];
Servo rightLeg[4];

int a;
int b;

int RH=90, RK=90, RA=90, RF=90;
int LH=90, LK=90, LA=90, LF=90;


void moveLeg(Servo leg[], int hipAngle, int kneeAngle, int ankleAngle, int footAngle) {
    leg[0].write(hipAngle);   // Move hip
    leg[1].write(kneeAngle);  // Move knee
    leg[2].write(ankleAngle); // Move ankle
    leg[3].write(footAngle);  // Move foot
}


void setup() {
  Serial.begin(9600);
  for(int i=0;i>=3;i++){
    a = i+3;
    b = i+7;
    leftLeg[i].attach(a);
    rightLeg[i].attach(b);
    /*
      * [0] = Hip
      * [1] = Knee
      * [2] = Ankle
      * [3] = Foot
    */
  }
}


void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Read the incoming string until a newline character
    command.trim();  // Remove any extra whitespace or newline characters
    
    if (command.startsWith("RH")) {
      RH = command.substring(2).toInt();  // Update the right hip angle
    }
    else if (command.startsWith("RK")) {
      RK = command.substring(2).toInt();  // Update the right knee angle
    }
    else if (command.startsWith("RA")) {
      RA = command.substring(2).toInt();  // Update the right ankle angle
    }
    else if (command.startsWith("RF")) {
      RF = command.substring(2).toInt();  // Update the right foot angle
    }
    else if (command.startsWith("LH")) {
      LH = command.substring(2).toInt();  // Update the left hip angle
    }
    else if (command.startsWith("LK")) {
      LK = command.substring(2).toInt();  // Update the left knee angle
    }
    else if (command.startsWith("LA")) {
      LA = command.substring(2).toInt();  // Update the left ankle angle
    }
    else if (command.startsWith("LF")) {
      LF = command.substring(2).toInt();  // Update the left foot angle
    }
    moveLeg(rightLeg, RH, RK, RA, RF);  // Move the right leg update
    moveLeg(leftLeg, LH, LK, LA, LF);   // Move the left leg update
  }
}
