#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define pulseMin 95
#define pulseMax 540

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int Sdata;//ServoData
int Tdata;//TranmiterData

String oldCommand;
String actCommand;
String newCommand;


int walkMoveList[][4]={
  {135, 45, 45, 135},
  {90,  0 , 90, 0  },
  {90,  45, 90, 135},
  {90,  0 , 90, 0  },
};

int fowardWalkMoveList[][4]={
  {90,  0 , 90, 0  },
  {90,  45, 90, 135},
  {90,  0 , 90, 0  },
  {135, 45, 45, 135},
};


int totalSizeWalkMoveList = sizeof(walkMoveList); // Get the total size of the array in bytes
int elementSizeWalkMoveList = sizeof(walkMoveList[0][0]); // Get the size of one element in bytes
int numElementsWalkMoveList = totalSizeWalkMoveList / elementSizeWalkMoveList; // Calculate the number of elements

int i;

unsigned short SApos = 0, SBpos = 0, SCpos = 0, SDpos = 0;
unsigned short SVpos[]{SApos = 0, SBpos = 0, SCpos = 0, SDpos = 0};


// Function to map servo angles to PWM pulse lengths
int angleToPulse(int angle) {
  return map(angle, 0, 180, pulseMin, pulseMax);
}

class IZIFunc{
public:
  
  bool Verify(){
    if(newCommand != actCommand){
      oldCommand = actCommand;
      actCommand = newCommand;
      Serial.print("Command: ");
      Serial.println(actCommand);
      pwm.setPWM(0, 0, angleToPulse(90)); // Move servo SA
      pwm.setPWM(1, 0, angleToPulse(180)); // Move servo SB
      return(true);
    }
  return(false);
  }

  void SoftWalk(int user, int angle){
    if(SVpos[user]<angle){
      for(i=SVpos[user];i>=angle;i++){
        pwm.setPWM(user, 0, angleToPulse(i));
        delay(10);
        SVpos[user] = i;
        Serial.println(i);
      }
    }
    else if(SVpos[user]>angle){
      for(i=SVpos[user];i<=angle;i--){
        pwm.setPWM(user, 0, angleToPulse(i));
        delay(10);
        Serial.println(i);
        SVpos[user] = i;
      }
    }
    else{Serial.println("No need to move any more!");}
  }

  void Walk(int moviment){
    if(moviment == 1){
    }
    Serial.print("walking?\nelements ");
    Serial.println(numElementsWalkMoveList);
    for(i=0;i<(numElementsWalkMoveList/4);i++){
      Serial.print("Walk Move ");
      Serial.println(i);
      Serial.println(walkMoveList[i][0]);
      Serial.println(walkMoveList[i][1]);
      SoftWalk(0, walkMoveList[i][0]);// Move servo SA
      //pwm.setPWM(0, 0, angleToPulse(walkMoveList[i][0])); 
      //pwm.setPWM(1, 0, angleToPulse(walkMoveList[i][1])); // Move servo SB
      
      SoftWalk(4, walkMoveList[i][2]);
      //pwm.setPWM(4, 0, angleToPulse(walkMoveList[i][2])); // Move servo SA2
      //pwm.setPWM(5, 0, angleToPulse(walkMoveList[i][3])); // Move servo SB2
      delay(1000);
    }
  }

  void Test(int Somting){
    if(Somting == 0){
      Serial.println("Test 'hi' has started!");
      for(i=0;i<=10;i++){
        pwm.setPWM(0, 0, angleToPulse(0));
        pwm.setPWM(1, 0, angleToPulse(0));
        delay(500);
        pwm.setPWM(0, 0, angleToPulse(180));
        pwm.setPWM(1, 0, angleToPulse(180));
        delay(500);
        Serial.println(i);
      }
      pwm.setPWM(0, 0, angleToPulse(90));
      pwm.setPWM(1, 0, angleToPulse(90));
      Serial.println("Test 'hi' has ended!");
    }

    if(Somting == 1){
      bool verify = Verify();
      Serial.println("Test 'Default' has startded!");
      for(i=0;i<=2;i++){
        Serial.println(i*2);
        pwm.setPWM((i*2), 0, angleToPulse(180));  // Move servo SA/SC
        Serial.println(i*2+1);
        pwm.setPWM(((i*2)+1), 0, angleToPulse(90));  // Move servo SB/SD
      }
      Serial.println("Test 'Default' has ended!");
      for(i=0;i<=4;i++){
        
      }
    }

    if(Somting == 2){
      Serial.println("Test 'Soft Moviment' has startded!");
      for(i=0;i<=160;i++){
        pwm.setPWM(0, 0, angleToPulse(i));  // Move servo SA/SC
        delay(10);
      }
    }
  }
};

IZIFunc Moves;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);  // Set frequency to 50 Hz (SG90 frequency) 
  delay(500);
}

void loop() {
  if (Serial.available() > 0) {
    newCommand = Serial.readStringUntil('\n');  // Read the incoming string until a newline character
    newCommand.trim();  // Remove any extra whitespace or newline characters
    
    if(newCommand.startsWith("SA")){ //SA = servo A
      Sdata = angleToPulse(newCommand.substring(2).toInt());
      pwm.setPWM(0, 0, Sdata);  // Move servo on channel 0
      Serial.print("SA: ");
      Serial.println(Sdata);
    }
    else if(newCommand.startsWith("SB")){ //SB = servo B
      Sdata = angleToPulse(newCommand.substring(2).toInt());
      
      pwm.setPWM(1, 0, Sdata);  // Move servo on channel 1
      Serial.print("SB: ");
      Serial.println(Sdata);
    }
    if(newCommand.startsWith("SC")){ //SA = servo A
      Sdata = angleToPulse(newCommand.substring(2).toInt());
      pwm.setPWM(4, 0, Sdata);  // Move servo on channel 0
      Serial.print("SA2: ");
      Serial.println(Sdata);
    }
    else if(newCommand.startsWith("SD")){ //SB = servo B
      Sdata = angleToPulse(newCommand.substring(2).toInt());
      
      pwm.setPWM(5, 0, Sdata);  // Move servo on channel 1
      Serial.print("SB2: ");
      Serial.println(Sdata);
    }
    else if(newCommand.startsWith("w")){
      Moves.Walk(1);
      //Serial.println("Walk Move");
    }
    else if(newCommand.startsWith("s")){
      Moves.Walk(2);
    }
    else if(newCommand.startsWith("Test")){
      Sdata = newCommand.substring(4).toInt();
      Serial.println(Sdata);
      Moves.Test(Sdata);
    }
    else if(newCommand.startsWith("sw")){
      Sdata = newCommand.substring(2).toInt();
      Moves.SoftWalk(0,Sdata);
      Moves.SoftWalk(4,Sdata);
      //Serial.println("Walk Move");
    }
    
    //delay(500);
    
    
    /*int Tdata = angleToPulse(data);
    pwm.setPWM(0, 0, Tdata);  // Move servo on channel 0
    Serial.println(Tdata);
    delay(1000);*/

/*    } else {
      Serial.println("Invalid angle. Please enter a value between 0 and 180.");
    }*/
  }
}
