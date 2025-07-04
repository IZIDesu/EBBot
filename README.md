
#### Project intro:
The objective of this project is to create a bipedal robot, that can be controlled by BT module/WiFi module/ and for advance to create a AI voice control. Im just trying to improve my abilitys and try someting more dificult to start but i will do my best on this! And my frase to create this without stoping is "I'm trying to do my impossibilities possible, trying to do overcome my capacities".

#### **MicroController**
<div style="color: red; font-weight: bold;">info:</div>
To be honest i just have arduino uno, so i just use arduino uno bc i have one already... and i dont realy know how to chose one acordly with what i need...

Arduino Uno 
~~Arduino Mega~~
~~ESP32~~

PWM controller -> PCA9685 -> adafruit device

#### **Controller**

To control the project i created one phone app, using MIT App Inventor, that sends commands to microcontroller via serial comunication


#### **Sensors**:7
<div style="color: red; font-weight: bold;">info:</div>
GYO sensor is necessary to make the strenght necessary to make a perfect move effect on the grownd. Potenciometer is necessary to make the servo knows it self where he is. Hall effect sensor is for the same effect as the potenciometer, but this sensor needs the 3D print to have some magnets. touch sensor of the foot to make the robot knows the ground where he is and move acordly with the help of the GYO. the camera is usefull to make the robot know where to go without coliding with halls and make the math needed to move acordly without damaging it self.

GYO sensor ~> [Torso]/[center of mass]
Potenciometer ~> [Servo motor]
~~hall effect sensor~~ ~> [Servo motor]
Touch Sensor ~> [foot]
Camera ~> [Face]
sound sensor ~> [Face]

#### **3D Print**
<div style="color: red; font-weight: bold;">info:</div>
This is a humanoid robot so... it have what we have... foot, legs, hands, arms, head...

##### Head:
Camera Suport/Space
Vision Sensor Suport/Space

##### Neck:
~~DC Motor~~                         <- too complicate
Servo Suport/Space

##### Torso:
**Info:**
I need to make the hip joint, in the down side of the torso, need to make the shoulder joint in the sides like right and left if the torso, and for the last, neck joint in the up side of the torso. inside of the torso i need to put the components devided to make the center of mass as the center of the torso as possible.

Controller Suport/Space
Servo Suport/Space
GYO sensor Suport/Space
Speaker Suport/Space
Microfone Suport/Space

##### Legs/arms:
<div style="color: red; font-weight: bold;">info:</div>
i need to make the joints far eneugh to make the servo rotate like our leg and make the close possible to make it light to make all the servos move well and to make the servos do less strength possible to consume less power. create the center of mass in the midle is important! create a perfect stability betwen the legs and in all joints to the foot/legs/Torso. create the leg with 4 servos to simulate partial of our leg moviments (knee joint/hip joint/leg rotation along with Z axis/leg rotation along with X/Y axis).

~~DC Motor~~                        <- too complicate
~~hall effect sensor Suport~~ <- too complicate
Servo Suport/space


#### Foot:
<div style="color: red; font-weight: bold;">info:</div>
I need to make the center mass more in one side than the other to make force to move the robot and to make the robot stable on the ground.
**Format:**
~~DC Motor~~
~~hall effect sensor Suport~~ <- too complicate
Servo Suport/space

#### Hands:
Servo Suport/space



<div style="color: #AAF0F0; font-weight: bold; font-size: 20px;">Code:</div>
<div style="color: red; font-weight: bold;">info:</div>
Well for the code is the easy and complicated part for some reason... well i'll program using Arduino ide and/or Visual Studios Code (VSC). I use arduino ide lastest version, bc is better than the older one, to complement that the lastest version is more user friendly and i can make it in dark mode. I use VSC with addons to compilate like arduino IDE, why dont i just use Arduino IDE, well i can, but the VCS to be more easy to write code and when im tired of the arduino IDE and i want to use somting diffrent... anyway when i start think about coding i think in the librarys that i will going to use and for that i search on internet to see if the servos, sensors and the other things have a better aplication about coding, and they have the servos have a library for them, but i will use a PWM controller so i will skip that one and use the controller library instead, bc i use arduino uno i dont have GPIO pins eneugh to put all the servos that i wanna use so the PCA9685 add me more 16 pins to put servos and i just spend space in the memory and not realy necessary pins like SCL (serial clock), SDA (Serial data), that are made for just I2C comunication, the other 12 digital GPIO pins are to the sensors and the 6 analog GPIO pins are to sensors too. and that is it!

<div style="color: #00F090; font-weight: bold; font-size: ;">Code Info:</div>

	PCA9685:
```C++ 
#include <Wire.h> /*<- used to I2C protocol comunication */
#include <Adafruit_PWMServoDriver.h> /*<- used to control the PCA9685*/
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); /*Creating PCA instance
```
	Servo PWM configuration:
```C++ 
#define pulseMin 95   /*minimum PWM pulse configuration
#define pulseMax 540  /*maximum PWM pulse configuration
```
	For multi-tasking i used millis variables:
```C++ 
int LastMillis = 0;
int DelayMillis = 100;
```
	Array to set a start position to all the servos:
```C++ 
int start[8]={0,180,90,0,180,0,90,0};
```
	Function to conver the 0~180º in PWM, to make more easy to understand where the servo is rotation:
```C++ 
int angleToPulse(int angle) {
  return map(angle, 0, 180, pulseMin, pulseMax);
}
```
	Void Setup:
```C++ 
  Serial.begin(9600);   //Start the serial comunication for debug at 9600 b/s
  pwm.begin();         // Start the PCA comunication
  pwm.setPWMFreq(50); //  Set PCA frequency to 50 Hz (SG90 frequency)
  delay(500);        //   to make sure that is everything ok
  for(i=0;i<=7;i++){//    for loop to make all the servos at the start position
    pwm.setPWM(i, 0, angleToPulse(SVpos[i])); // iterate for every servo and angle
  }
  Serial.println("Setup Compleat!"); /*debug to make sure everything is ok*/
```
	Void Loop:
```C++ 
  if (Serial.available() > 0) {
    newCommand = Serial.readStringUntil('\n');  // Read the incoming string until a newline character
    newCommand.trim();  // Remove any extra whitespace or newline characters
    if((millis()-LastMillis) >= DelayMillis){ // millis function or multi-tasking
      LastMillis = millis();                 // last value atualization

      if(newCommand.startsWith("a")){      //ex: moving a servo
        SVpos[0] = newCommand.substring(2).toInt(); //save to a vatiable the angle
        Sdata = angleToPulse(newCommand.substring(2).toInt());//conver to PWM
        pwm.setPWM(0, 0, Sdata);  // Move servo on channel 0
        Serial.print("a: ");     // Debug
        Serial.println(Sdata);  // Debug to check the a: corrent pos 
```







