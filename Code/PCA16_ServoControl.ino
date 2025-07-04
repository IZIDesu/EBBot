/*Na documentação é possivel encontrar alguns termos menos próprios ou desconhecidos, antes de já forneco algumas palavras que utilizo:
  I2C             -> inter integrated circuit (protocolo de Comunicação série)
  PWM             -> pulse width modulation / pulso de modulação de largura <- onda que fáz os servos mexerem que está definida entre 0~2ms high pulse de 20ms de pulso total(high/low)
  default stand   -> movimentos para se levantar pré-definido/ robo apenas levantado.
  fedback         -> resposta/ informação recebida por fazer algo
  TMP             -> temporario/ temporary
  variavel global -> variavel que pode ser utilizada no código todo
  array           -> estrutura de dados que consiste na coleção de elementos do mesmo tamanho de armazenamento
  String          -> como o nome diz é uma teia que comsegue armazenar elementos de difrentes tamanhos e difrentes categorias num só elemnto (ex: String a; int b = 2; a = "the number is " + String(b)) // neste exemplo 'a' fica "the number is 2"
  millis          -> uma função do arduino que utiliza o clock interno para determinar quanto tempo passou em milisegundos
  função millis   -> com base na função "millis" é possivel realizar "multi-tasking" (ex: int LastMillis = 0; int DelayMillis = 100; if(millis()-LastMillis >= DelayMillis){LastMillis=millis()})
  other           -> outro
  bytes           -> conjunto de 8 bites
  return x        -> a função retorna com o valor de x (ex: a = int func(){b = 1;return b}) neste exemplo a função "int func" retorna o b que por sua vês é 1
*/



/*o "#include x" é utilizado para juntar ficheiros ao codigo presente!
  no meu caso estou a importar bibliotecas que são códigos de pessoas/ impresas
  Wire.h bilbioteca fornece-me a comunicação I2C
  Adafruit_PWMServoDriver.h fornece-me a comunicação com aplaca PCA9685
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//o "#define x y" é utilizado para sempre que vir o "x" no código substituir por "y" \
  neste caso estou a definir o pulso máximo e minimo do PWM que descobri a tentativa e erro
#define pulseMin 95
#define pulseMax 540

//criar uma imagem da biblioteca Adafruit_PWMServoDriver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//as variaveis globais abaixo são apenas variaveis que utilizo para guardar dados TMP
int Sdata;//ServoData
int Tdata;//OtherData
int Odata;//OtherData
int Ldata;//OtherData

//as strings global abaixo são para trocar valores entre o novo, actual e o velho
String oldCommand;
String actCommand;
String newCommand;

//variavel global de texto
String Text;

//variaveis para a função millis
int LastMillis = 0;
int DelayMillis = 100;


/*
int walkMoveList[][8] = {
//Left               ||Right
  {  0,   0,  85,   0,  180, 180,  95,   0},  // Step 1
  {  0,  45,  85,   0,  180, 135,  95,   0},  // Step 2
  {  0,   0,  85,   0,  180, 180,  95,   0},  // Step 3

  {  0,   0, 120,   0,  180, 180,  60,   0},  // Step 4
  { 45,   0,  85,   0,  135, 180,  95,   0},  // Step 5
  {  0,   0,  85,   0,  180, 180,  95,   0}   // Step 6
};//AE,  JE,   PE,  OE,  AD,  JD,  PD,  OD   // servo name

{ 80,  50, 120,   0, 110, 150,  70,   0},
//{ 90,   0,  80,   0,  90, 180,  95,   0},  // Step 1
  { 40,  45, 120,   0, 135, 180,  70,   0},  // Step 2
  { 80,  45, 120,   0, 110, 180,  40,   0},  // Step 3

  {140,   0, 155,   0,  70, 135,  70,   0},  // Step 4
  {140,   0, 155,   0,  70, 135,  70,   0},  // Step 5
  { 80,  50, 120,   0, 110, 150,  70,   0}   // Step 6
};*/


/*
o array seguinte é utilizado para ter as coordenadas para 
que o robo ande para a frente.
o abaixo é para que ande para trás "backward"
o array "end" contem as posições dos servos quando estiver a arrumar o robo 
o array "start" é o array que contem o default stand
*/
/*CLOSE TRY
  { 80,  50, 120,   0,  70, 140,  70,   0},
  { 40,  45,  70,   0,  50, 160,  70,   0},  // Step 2
  { 80,  45, 120,   0,  50, 180,  70,   0},  // Step 3

  {140,   0, 155,   0, 110, 150,  70,   0},  // Step 4
  {140,   0, 155,   0, 135, 135,  70,   0},  // Step 5
  { 80,  50, 120,   0, 110, 140,  70,   0}   // Step 6
*/

//80,50,120,0,110,140,70,0
//ws0;0;500

//AD = 120 \
  JD = 150  \
  
//AE = 60  \
  JE = 50   \

/*
int walkMoveList[][8] = { //           <-------------------------- needs a upgrade to walk better! 
  //Left              ||Right
//{ 60,  50, 120,   0, 120, 150,  70,   0},
  { 40,  45, 120,   0,  80, 160,  70,   0},  // Step 1  
  { 40,  45, 120,   0,  80, 180,  70,   0},  // Step 2  
  { 60,  45, 110,   0, 100, 160,  70,   0},  // Step 3

  { 90,   0, 100,   0, 110, 150,  70,   0},  // Step 4
  {100,   0,  90,   0, 135, 135,  70,   0},  // Step 5 <- needs a change
  { 60,  50, 120,   0, 120, 150,  70,   0}   // Step 6
};//AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD    // servo name\
anca, joelho, pé, outro,anca, joelho, pé, outro
*/
int TMP[][8] = {
  //AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD    // servo name
  { 90,   0,  90,   0,  90,  180, 90,  0},
  { 90,   0,  90,   0,  90,  180, 90,  0},
  { 60,  50, 120,   0, 120,  130, 50,  0},
  { 60,  50, 120,   0, 120,  130, 50,  0}
};
/*
int walkMoveList[][8] = { //           <-------------------------- needs a upgrade to walk better! 
  //Left              ||Right
//{ 60,  50, 120,   0, 120, 150,  70,   0}, { 70,  30, 110,   0, 110, 150,  70,   0},
  
  { 70,  30, 120,   0, 110, 150,  60,   0},  // Step 1  
  { 90,  35, 130,   0, 110, 150,  90,   0},  // Step 2  
  { 70,  30, 120,   0, 110, 150,  60,   0},  // Step 3

  { 70,  30,  90,   0,  90, 145,  50,   0},  // Step 4
  { 70,  30, 120,   0, 110, 150,  60,   0},  // Step 5 <- needs a change
  { 70,  30, 120,   0, 110, 150,  60,   0}   // Step 6
};//AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD    // servo name\
*/

int walkMoveList[][8] = { //           <-------------------------- needs a upgrade to walk better! 
  //Left              ||Right
//{ 60,  50, 120,   0, 120, 150,  70,   0}, { 70,  30, 110,   0, 110, 150,  70,   0},
  /*
  { 70,  30, 120,   0, 110, 150,  70,   0},  // Step 1  
  {100,  30, 120,   0, 130, 150,  70,   0},  // Step 3
  { 70,  30, 120,   0, 110, 150,  70,   0},  // Step 5
  { 50,  30, 120,   0,  90, 150,  70,   0},  // Step 7
  { 70,  30, 120,   0, 110, 150,  70,   0}   // Step 9*/
  { 80,  50, 120,   0,  70, 140,  70,   0},
  { 40,  45,  70,   0,  50, 160,  70,   0},  // Step 2
  { 80,  45, 120,   0,  50, 180,  70,   0},  // Step 3
  {140,   0, 155,   0, 110, 150,  70,   0},  // Step 4
  {140,   0, 155,   0, 135, 135,  70,   0},  // Step 5
  { 80,  50, 120,   0, 110, 140,  70,   0}   // Step 6
};//AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD    // servo name\
//AE90  //JE35 //PE130 || AD110 //JD150 //PD90 

int wwns[][] = {
  //90,   0,  90,   0,  90, 180,  90,   0
  { 90,   0,  90,   0,  90, 180,  90,   0},
  { 95,   0,  90,   0,  85, 180,  90,   0},
  { 90,   0,  90,   0,  90, 180,  90,   0},
  { 85,   0,  90,   0,  95, 180,  90,   0},
  { 90,   0,  90,   0,  90, 180,  90,   0},
  
  { 90,   0,  90,   0,  90, 180,  90,   0},
  { 90,   0,  90,   0,  90, 180,  90,   0},
  { 90,   0,  90,   0,  90, 180,  90,   0},

  { 60,  50, 120,   0, 120, 130,  50,   0},
  { 65,  50, 120,   0, 115, 130,  50,   0},
  { 60,  50, 120,   0, 120, 130,  50,   0},
  { 55,  50, 120,   0, 125, 130,  50,   0},
  { 60,  50, 120,   0, 120, 130,  50,   0},
  
};//AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD    // servo name\

int ducking[][8] = { //           <-------------------------- needs a upgrade! 
  //Left              ||Right
  {60, 50,120,  0,120,130, 50,  0},
  {60, 50,120,  0,120,130, 50,  0},
  {60, 50,120,  0,120,130, 50,  0}
};//AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD    // servo name\
 

int IZIWalkMoveList[][8] = { //         <-------------------------- no servo test! 
  //Left              ||Right
//{ 60,  50, 120,   0, 120, 150,  70,   0},  
  { 40,  45, 120,   0,  80, 160,  70,   0},  // Step 1  
  { 40,  45, 120,   0,  80, 180,  70,   0},  // Step 2  
  { 60,  45, 110,   0, 100, 160,  70,   0},  // Step 3

  { 90,   0, 100,   0, 110, 150,  70,   0},  // Step 4
  {100,   0,  90,   0, 135, 135,  70,   0},  // Step 5 <- needs a change
  { 60,  50, 120,   0, 120, 150,  70,   0}   // Step 6
};//AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD    // servo name\
anca, joelho, pé, outro,anca, joelho, pé, outro

int backwardWalkMoveList[][8]={
  //Left               ||Right
  {  0,   0, 120,   0, 180, 180,  95,   0},  // Step 1
  { 45,   0, 120,   0, 180, 135,  95,   0},  // Step 2
  {  0,   0,  85,   0, 180, 180,  95,   0},  // Step 3

  {  0,   0,  85,   0, 180, 180,  95,   0},  // Step 4
  {  0,  45,  85,   0, 135, 180,  60,   0},  // Step 5
  {  0,   0,  85,   0, 180, 180,  95,   0}   // Step 6
};//AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD      // servo name

//robot with torso test moves
int forwardMove[][8]={
  //Left              ||Right
  { 90,   0,  80,   0,  90, 180,  95,   0},  // Step 1
  { 70,  20,  80,   0, 115, 180,  95,   0},  // Step 2
  { 90,  45, 100,   0, 135, 155,  60,   0},  // Step 3

  {120,  20, 115,   0,  90, 135,  95,   0},  // Step 4
  {120,   0, 115,   0,  60, 135,  95,   0},  // Step 5
  { 90,   0,  80,   0,  90, 180,  95,   0}   // Step 6
};//AE,  JE,   PE,  OE,  AD,  JD,  PD,  OD    // servo name

int forwardMove2[][8]={
  //Left              ||Right
  { 90,   0,  80,   0,  90, 180,  95,   0},  // Step 1
  { 70,  20,  80,   0, 115, 180,  95,   0},  // Step 2
  { 90,  45, 100,   0, 135, 165,  60,   0},  // Step 3
  { 90,  45, 100,   0, 115, 155,  60,   0},   // Step 4

  {110,  20, 115,   0,  90, 135,  95,   0},  // Step 5
  {120,   0, 115,   0,  60, 135,  95,   0},  // Step 6
  { 90,   0,  80,   0,  90, 160,  95,   0},  // Step 7
  { 90,   0,  80,   0,  90, 180,  95,   0}   // Step 8
};// AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD   // servo name

int sitDown[][]={
  { 90,   0,  90,   0,  90, 180,  90,   0},
  { 90,   0,  90,   0,  90, 180,  90,   0},
  { 90,   0,  90,   0,  90, 180,  90,   0},
};

int seatLegDown[]={40, 180,  90,  90, 150,   0,  90,  90};
//                 AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD   // servo name

//array que contem as posições iniciais do robo
int end[8]={  0,  55,  20,   0,   0, 145, 160,   0};
//           AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD   // servo name

//array que contem as posições iniciais do robo "default stand"
int start[8]={ 80,  30, 120,   0, 100, 150,  60,   0};
//             AE,  JE,  PE,  OE,  AD,  JD,  PD,  OD   // servo name

///////////////////////////////////////////////////////// Tenta fazer Stick e mudar só a posição das ancas em sentidos opostos para ver se o torso fica sempre a 90º do chao se não der ardeu tudo!!!
int Stick[8]={ 90,  0, 90,  0, 90,180, 90,  0};
int Duck[8] ={ 60, 40,120,  0,120,140, 70,  0};

int Test01[8]={100,  30, 120,   0, 130, 150,  70,   0};
int Test02[8]={ 50,  30, 120,   0,  90, 150,  70,   0};

//WS function variable
int intermediatePosition;

//JE55 , PE20
//JD145, PD160

//obtem o tamanho do array em bytes
int totalSizeWalkMoveList = sizeof(walkMoveList);
//obtem o tamanho de um elemento em bytes
int elementSizeWalkMoveList = sizeof(walkMoveList[0][0]);
//calculo do numer dos elementos do array 
short numElementsWalkMoveList = totalSizeWalkMoveList / elementSizeWalkMoveList;

/*função que escolhe qual o array de movimento a ser utilizado
int listNumber -> é o numero que escolhe o array*/

int arrayNum(int listNumber){
  //iniciar as variaveis
  int totalSize = 0;
  int elementSize = 0;
  /* "switch" está a fazer a vês dos: 
  "if(listNumber == x){
    totalSize = sizeof(walkMoveList);    
    elementSize = sizeof(walkMoveList[0][0]);
    break;}
    else if(listNumber == x){...}"
  */
  switch(listNumber){
    case 0:
      totalSize = sizeof(walkMoveList);
      elementSize = sizeof(walkMoveList[0][0]);
      //o break é utilizado para parar o funcionamento do switch
      break;
    case 1:
      totalSize = sizeof(backwardWalkMoveList);
      elementSize = sizeof(backwardWalkMoveList[0][0]);
      //o break é utilizado para parar o funcionamento do switch
      break;
    case 2:
      totalSize = sizeof(seatLegDown);
      elementSize = sizeof(seatLegDown[0]);
      //o break é utilizado para parar o funcionamento do switch
      break;
    case 3:
      totalSize = sizeof(end);
      elementSize = sizeof(end[0]);
      //o break é utilizado para parar o funcionamento do switch
      break;
    case 4:
      totalSize = sizeof(start);
      elementSize = sizeof(start[0]);
      //o break é utilizado para parar o funcionamento do switch
      break;
    case 5:
      totalSize = sizeof(Stick);
      elementSize = sizeof(Stick[0]);
      break;
    case 6:
      totalSize = sizeof(Duck);
      elementSize = sizeof(Duck[0]);
      break;
    case 7:
      totalSize = sizeof(ducking);
      elementSize = sizeof(ducking[0]);
      break;
    case 8:
      totalSize = sizeof(wwns);
      elementSize = sizeof(wwns[0]);
      break;


    case 99:
      totalSize = sizeof(TMP);
      elementSize = sizeof(TMP[0]);
      break;
  }
  
  // a variavel numElements, faz o calculo para obter o numero de elementos presentes no array selecionado 
  int numElements = totalSize / elementSize;
  //a seguinte documentação é utilizada para testes de debug:
  /*Text = "listNumber: " + String(listNumber) + "\nTotalSize: " + String(totalSize) + "\nelementSize: " + String(elementSize) + "\nnumElements: " + String(numElements); 
  Serial.println(Text);
  Text = "totalSizeWalkMoveList: " + String(totalSizeWalkMoveList) + "\nelementSizeWalkMoveList: " + String(elementSizeWalkMoveList) + "\nnumElementsWalkMoveList: " + String(numElementsWalkMoveList);
  Serial.println(Text);
  totalSize = sizeof(walkMoveList); // Get the total size of the array in bytes
  elementSize = sizeof(walkMoveList[0][0]); // Get the size of one element in bytes
  Text = "\nTotalSize: " + String(totalSize) + "\nelementSize: " + String(elementSize);
  Serial.println(Text);
  */

  //retorna o numero de elementos do array selecionado
  return(numElements);
}

int i;

//as variaveis seguintes são as variaveis de fedback para poder controlar e saber em que angulo cada servo está
//    anca,      joelho,    Pé,        outro    ||
short AEpos = 0, JEpos = 0, PEpos = 0, OEpos = 0, ADpos = 0, JDpos = 0, PDpos = 0, ODpos = 0;
//    AE       , JE       , PE       , OE       , AD       , JD       , PD       , OD      \
      esquerda , esquerdo , esquerdo , esquerdo , direita  , direito  , direito  , direito 

//array que contem as posições atuais de cada servo
//int SVpos[8] = {AEpos = 90, JEpos = 0, PEpos = 80, OEpos = 0, ADpos = 90, JDpos = 180, PDpos = 95, ODpos = 0};
int SVpos[8] = {AEpos = 80, JEpos = 30, PEpos = 120, OEpos = 0, ADpos = 100, JDpos = 150, PDpos = 70, ODpos = 0};
//60,  50, 120,   0, 120, 150,  70,   0
//AE =80\\
  JE =50\\
  PE =120

int SVcommand[3] = {0,0,0};

//função que mapeia os angulos dos servos em pulsos de largura de PWM
int angleToPulse(int angle) {
  return map(angle, 0, 180, pulseMin, pulseMax);
}


//A class a baixo é a main class onde contem todas as funções que fazem o robo andar e mais algumas funções de debug
class IZIFunc{
  
//coisas que não são disponiveis fora da class
private:
  //teste para verificar se um comando foi mandado enquanto está a ser realizada uma função
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

//coisas que são disponiveis fora da class
public:
  /*função SoftWalk(int user, int angle)
  o principio da função está no nome realizar um movimento soft/ suave apenas num servo
  int user  -> o servo
  int angle -> o angulo que é pretendido o servo mexer
  Nota: esta função foi utilizada para aprender como se realiza um movimento soft.
  */
  void SoftWalk(int user, int angle){
    Text = "User:" + String(user);
    Serial.println(user);
    if(SVpos[user]<angle){
      Serial.println("Bigger!");
      for(int i=SVpos[user];i<=angle;i++){
        /*Serial.println(SVpos[user]);
        Serial.println(i);*/
        pwm.setPWM(user, 0, angleToPulse(i));
        SVpos[user] = i;
        /*Text = "User: "+String(user) + "\nSVpos: "+String(SVpos[user]);
        Serial.println(Text);*/
      }
    }
    else if(SVpos[user]>angle){
      Serial.println("Smaller!");
      for(int i=SVpos[user];i>=angle;i--){
        /*Serial.println(SVpos[user]);
        Serial.println(i);*/
        pwm.setPWM(user, 0, angleToPulse(i));
        SVpos[user] = i;
        /*Text = "User: "+String(user) + "\nSVpos: "+String(SVpos[user]);
        Serial.println(Text);*/
      }
    }
    else{Serial.println("No need to move any more!");}
  }

  /*função Walk(int velocity=100)
  o principio da função é realizar uma sequencia de movimentos seguidos
  velocity -> velocidade a executar cada conjunto de linhas do array
  Nota: esta função foi para poder aprender como os servos funcionam com sequencias de movimentos.
  */
  void Walk(int velocity=100){ /*int movement*/

    Text = "Walk Velo: "+velocity;
    Serial.println(Text);
    totalSizeWalkMoveList = sizeof(walkMoveList); // Get the total size of the array in bytes
    elementSizeWalkMoveList = sizeof(walkMoveList[0][0]); // Get the size of one element in bytes
    numElementsWalkMoveList = totalSizeWalkMoveList / elementSizeWalkMoveList; // Calculate the number of elements
    Serial.print("walking?\nelements ");
    Serial.println(numElementsWalkMoveList/4);
    for(i=0;i<((numElementsWalkMoveList/4)-1);i++){
      Serial.print("Walk Move ");
      Serial.println(i);
      Serial.println(walkMoveList[i][0]);
      Serial.println(walkMoveList[i][1]);
      //SoftWalk(user, Angle, velocity)
      //SoftWalk(0, walkMoveList[i][0]);// Move servo SA
      //SoftWalk(1, walkMoveList[i][1]);// Move servo SB
      pwm.setPWM(0, 0, angleToPulse(walkMoveList[i][0]));  // Move servo SA/ AE
      pwm.setPWM(1, 0, angleToPulse(walkMoveList[i][1]));  // Move servo SB/ JE
      pwm.setPWM(2, 0, angleToPulse(walkMoveList[i][2]));  // Move servo SC/ PE
      pwm.setPWM(3, 0, angleToPulse(walkMoveList[i][3]));  // Move servo SD/ OE
      //tem que se colocar todos para andarem ao mesmo tempo! T-T
      delay(velocity);
    }
  }
  
  /*função WS ou WalkStep(move=0, velocity=10, iterations=100)
  o objetivo desta função é poder ter um controlo estável dos movimentos do robo 
  colocando-os mais lentos ou com mais iterações para fazer um movimento mais suave.
  */

  void WS(int move = 0, int velocity = 10, int iterations = 100, int fase = 0, int fase2 = 0, int servoToFase1 = 99, int servoToFase2 = 99) {
    /*Text é uma variavel global para texto de debug. neste caso está só a dár a informação do movimento, delay entre cada 
    movimento/animação, iterations/iterações que é a quantidade de iterações necessario para fazer um determinado angulo.
    avisa também quando é que a animação/movimento está começar.
    diz a quantia de elementos da lista/ sequencia/ move array selecionado
    */
    Text = "move: "+String(move)+"\ndelay: "+String(velocity)+"\niterations: "+String(iterations)+"\nfase: "+String(fase)+"\nfase2: "+String(fase2)+"\nServoToFase1: "+String(servoToFase1)+"\nServoToFase2: "+String(servoToFase2);
    Serial.println(Text);
    Serial.println("Starting Smooth Walk Sequence");
    int numElementsList = arrayNum(move);
    Serial.print("numELementsList:");
    Serial.println(numElementsList);
    
    //itera sobre cada passo na lista/ sequencia/ move array selecionado
    for (int step = 0; step < ((numElementsList / 8)+fase); step++) {  // Update loop por 8 servos
      int targetPositions[8];    // Now handling 8 servos
      int startPositions[8];    // Now handling 8 servos
      int steps = iterations;  // Number of intermediate steps for smoothness
     // Obtem o começo e as posições alvo para cada servo
      
      for (int servo = 0; servo < 8; servo++) {  // Update loop to 8 servos
        startPositions[servo] = SVpos[servo];
        if(move == 0){//array para a frente
          targetPositions[servo] = walkMoveList[step][servo];
        }
        else if(move == 1){ //array para tráz
          targetPositions[servo] = backwardWalkMoveList[step][servo];
        }
      }
      // Move cada servo gradualmente para a sua respetiva posição em sincronia
      for (int increment = 0; increment <= steps; increment++) {
        
        for (int servo = 0; servo < 8; servo++) {  // Update loop to 8 servos
          if(servoToFase1 != 99 && servo == servoToFase1){
            int intermediatePosition = startPositions[servo] + ((targetPositions[servo]+fase) - startPositions[servo]) * increment / steps;
          }
          else if(servoToFase2 != 99 && servo == servoToFase2){
            int intermediatePosition = startPositions[servo] + ((targetPositions[servo]+fase2) - startPositions[servo]) * increment / steps;
          }
          else{
            int intermediatePosition = startPositions[servo] + (targetPositions[servo] - startPositions[servo]) * increment / steps;
          }
          /*// Debugging: Print intermediate positions for servos
          if (servo == 0) {  // Example: print for Servo 2 for debugging
            Serial.print("S1: ");
            Serial.println(intermediatePosition);
            Serial.print("steps: ");
            Serial.println(steps);
          }*/
          pwm.setPWM(servo, 0, angleToPulse(intermediatePosition));
          SVpos[servo] = intermediatePosition;  // Update a posição atual
        }
        delay(velocity);  // Slightly longer delay to ensure smooth movement
      }
    }
    //debug para saber que a sequencia acabou
    Serial.println("Smooth move is completed!!!");
  }


  void AutoOptimize(int move, int servoTarget, int stepSize = 1, int maxSteps = 30) {
    int bestFase = 0;

    for (int i = 0; i < maxSteps; i++) {
      int faseTest = i * stepSize;
      Serial.println("===================");
      Serial.print("Testing fase: ");
      Serial.println(faseTest);

      // Run animation with current fase for selected servo
      WS(move, 10, 100, faseTest, 0, servoTarget, 99);

      // OPTIONAL: Add a sensor-based or visual check here
      // For now, you could pause and manually observe
      delay(2000);

      // You can add Serial input or a sensor reading to determine "stable"
      // if (isStable()) {
      //   bestFase = faseTest;
      //   break;
      // }
    }

    Serial.print("Best fase found: ");
    Serial.println(bestFase);
  }

  void AutoOptimize2(int move, int servoTarget, int stepSize = 1, int maxSteps = 30) {
    Serial.println("===== Auto Optimization Started =====");
    int bestFase = 0;

    for (int i = 0; i < maxSteps; i++) {
      int faseTest = i * stepSize;
      Serial.println("===================");
      Serial.print("Testing fase: ");
      Serial.println(faseTest);

      // Run your animation with the current test phase
      WS(move, 10, 100, faseTest, 0, servoTarget, 99);

      Serial.println("Observe movement.");
      Serial.println("Type 's' to save this phase as best, or press enter to continue...");

      // Wait for user input from Serial Monitor
      while (!Serial.available()) {
        delay(100);  // Give time to react
      }

      char input = Serial.read();
      Serial.read(); // Clear newline
      if (input == 's' || input == 'S') {
        bestFase = faseTest;
        Serial.println("Best phase SAVED!");
        break;
      } else {
        Serial.println("Continuing...");
      }
    }
  }

  /*função Test(somting)
  esta função serve apenas para debug e para resolução de alguns problemas que foi adquirindo ao longo do código
  */
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

/*criar uma imagem da minha class chamando a Moves, isto quer dizer, quando no codigo aparcer "Moves."algo" " significa que me refiro á minha class*/
IZIFunc Moves;

/*função setup() ou função de configurações
tem objetivo de fazer todas as configurações.
a funcção só é executada uma vês, porque eu não a chamo em mais nenhuma parte do código
*/
void setup() {
  //inicia a comunicação Série a 9600b/s
  Serial.begin(9600);
  //inicia a comunicação com o PCA9685
  pwm.begin();
  /*
  define a frequência do PCA9685 a 50Hz
  50Hz é a frequência do servo motor SG90
  */
  pwm.setPWMFreq(50);
  //delay para estabilizar o PCA
  delay(500);
  //colocar todos os servos para a posição inicial (default stand)
  for(i=0;i<=7;i++){
    pwm.setPWM(i, 0, angleToPulse(SVpos[i]));
  }
  //debug para saber que as configurações acabaram
  Serial.println("Set Up Compleat!");
}


void loop() {

  //começa por verificar se recebe valores pela comunicação série (Serial Monitor, BT)
  if (Serial.available() > 0) {
    //lê a string do valor da porta série e coloca o numa variavel até aparecer o caracter de newline -> '\n'
    newCommand = Serial.readStringUntil('\n');
    //remove os espaços da string ou os caracteres de "newline('\n')"
    newCommand.trim();
    //a seguinte verificação é utilizada para poder fazer multi-tasking
    if((millis()-LastMillis) >= DelayMillis){ // função millis está imbotida no arduino.
      LastMillis = millis(); // atualização do last millis para um novo ultimo
      //as verificações a seguir verifica se a comunicação série começa com "string" para executar diversas funções

      if(newCommand.startsWith("AE")){      //AE = anca esquerda pino 0
        
        SVcommand[0] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(0, 0, Sdata);  // Move servo on channel 0
        Serial.print("AE: ");
        Serial.println(Sdata);
      }
      else if(newCommand.startsWith("JE")){ //JE = Joelho esquerdo pino 1
        SVcommand[1] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(1, 0, Sdata);  // Move servo on channel 1
        Serial.print("JE: ");
        Serial.println(Sdata);
      }
      else if(newCommand.startsWith("PE")){ //PE = pé esquerdo pino 2
        SVcommand[1] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(2, 0, Sdata);  // Move servo on channel 1
        Serial.print("PE: ");
        Serial.println(Sdata);
      }
      else if(newCommand.startsWith("OE")){ //PE = pé esquerdo pino 2
        SVcommand[1] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(3, 0, Sdata);  // Move servo on channel 1
        Serial.print("OE: ");
        Serial.println(Sdata);
      }

      else if(newCommand.startsWith("AD")){ //AD = anca direita pino 4
        SVcommand[2] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(4, 0, Sdata);  // Move servo on channel 4
        Serial.print("AD: ");
        Serial.println(Sdata);
      }
      else if(newCommand.startsWith("JD")){ //JD = Joelho direita pino 5
        SVcommand[3] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(5, 0, Sdata);  // Move servo on channel 5
        Serial.print("JD: ");
        Serial.println(Sdata);
      }
      else if(newCommand.startsWith("PD")){ //PD = pé direita pino 6
        SVcommand[3] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(6, 0, Sdata);  // Move servo on channel 5
        Serial.print("PD: ");
        Serial.println(Sdata);
      }
      else if(newCommand.startsWith("OD")){ //PD = pé direita pino 6
        SVcommand[3] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(7, 0, Sdata);  // Move servo on channel 5
        Serial.print("OD: ");
        Serial.println(Sdata);
      }

      //Move 2 servos at the same time at the same angle
      else if(newCommand.startsWith("AA")){ //AMBOS AS ANCAS
        SVcommand[0] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        Tdata = angleToPulse((newCommand.substring(2).toInt()-180)*(-1));
        pwm.setPWM(0, 0, Sdata);  // Move servo on channel 0
        pwm.setPWM(4, 0, Tdata);  // Move servo on channel 4
        Serial.print("AE: ");
        Serial.println(newCommand.substring(2).toInt());
        Serial.print("AD: ");
        Serial.println((newCommand.substring(2).toInt()-180)*(-1));
      }
      else if(newCommand.startsWith("JJ")){ //AMBOS OS JUELHOS
        SVcommand[0] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        Tdata = angleToPulse((newCommand.substring(2).toInt()-180)*(-1));
        pwm.setPWM(1, 0, Sdata); 
        pwm.setPWM(5, 0, Tdata); 
        Serial.print("JE: ");
        Serial.println(newCommand.substring(2).toInt());
        Serial.print("JD: ");
        Serial.println((newCommand.substring(2).toInt()-180)*(-1));
      }
      else if(newCommand.startsWith("PP")){ //AMBOS OS PÉS
        SVcommand[0] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        Tdata = angleToPulse((newCommand.substring(2).toInt()-180)*(-1));
        pwm.setPWM(2, 0, Sdata); 
        pwm.setPWM(6, 0, Tdata); 
        Serial.print("PE: ");
        Serial.println(newCommand.substring(2).toInt());
        Serial.print("PD: ");
        Serial.println((newCommand.substring(2).toInt()-180)*(-1));
      }

      //Move 2 servos at the same time at oposite angles
      else if(newCommand.startsWith("aA")){ //AMBOS AS ANCAS
        SVcommand[0] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(0, 0, Sdata);  // Move servo on channel 0
        pwm.setPWM(4, 0, Sdata);  // Move servo on channel 4
        Serial.print("AE && AD: ");
        Serial.print(Sdata);
      }
      else if(newCommand.startsWith("jJ")){ //AMBOS OS JUELHOS
        SVcommand[0] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(1, 0, Sdata); 
        pwm.setPWM(5, 0, Sdata); 
        Serial.print("JE && JD: ");
        Serial.println(Sdata);
      }
      else if(newCommand.startsWith("pP")){ //AMBOS OS PÉS
        SVcommand[0] = newCommand.substring(2).toInt();
        Sdata = angleToPulse(newCommand.substring(2).toInt());
        pwm.setPWM(2, 0, Sdata); 
        pwm.setPWM(6, 0, Sdata); 
        Serial.print("PE && PD: ");
        Serial.println(Sdata);
      }

      
      else if(newCommand.startsWith("ww")){ //walk funcion
        Sdata = newCommand.substring(2).toInt();
        Moves.Walk(Sdata);
      }

      else if(newCommand.startsWith("ws")){ //----------(Ex:ws1;10;100)
        sscanf(newCommand.c_str(), "ws%d;%d;%d", &Sdata, &Tdata, &Odata);
        //sscanf analiza a string newCommand e vai guardando os valores quando encontra \
        "ws", ";", ";" e guarda nas variaveis Sdata,Tdata e Odata respetivamente
        /*        move, delay, iterations-> default 100*/
        Moves.WS(Sdata, Tdata, Odata);
      }

      else if(newCommand.startsWith("Test")){// testes/debug
        Sdata = newCommand.substring(4).toInt();
        Serial.println(Sdata);
        Moves.Test(Sdata);
      }
      else if(newCommand.startsWith("sw")){//----------(Ex:sw3;180)
        Sdata = newCommand.substring(2, newCommand.indexOf(';')).toInt();
        Tdata = newCommand.substring(newCommand.indexOf(';') + 1).toInt();
        Text = "Sdata: " + String(Sdata) + "\nTdata: " + String(Tdata);
        Serial.println(Text);
        Moves.SoftWalk(Sdata, Tdata);
        //             servo, angle
      }
      else if(newCommand.startsWith("info")){// debugs
        for(i=0;i<=3;i++){
          Text = "Servo" + String(i) + ": " + String(SVpos[i]);
          Serial.println(Text);
        }
      }
      else if(newCommand.startsWith("ListUpdate")){// debugs
        totalSizeWalkMoveList = sizeof(walkMoveList); // Get the total size of the array in bytes
        elementSizeWalkMoveList = sizeof(walkMoveList[0][0]); // Get the size of one element in bytes
        numElementsWalkMoveList = totalSizeWalkMoveList / elementSizeWalkMoveList; // Calculate the number of elements
        Text = "Total: "+String(totalSizeWalkMoveList)+"\nNumElements"+String(numElementsWalkMoveList);
        Serial.println(Text);
      }
      else if(newCommand.startsWith("end")){// posição do fim
        Serial.println("end Started!");
        for(int i=0;i<8;i++){
          //Serial.println(end[i]);
          pwm.setPWM(i, 0, angleToPulse(end[i]));
        }
      }
      else if(newCommand.startsWith("start")){// posição inical/ default stand
        Serial.println("start Started!");
        for(int i=0;i<8;i++){
          //Serial.println(start[i]);
          pwm.setPWM(i, 0, angleToPulse(start[i]));
        }
      }
      else if(newCommand.startsWith("st")){// Stick Move
        Serial.println("Stick Move Started!");
        for(int i=0;i<8;i++){
          //Serial.println(Stick[i]);
          pwm.setPWM(i, 0, angleToPulse(Stick[i]));
        }
      }
      else if(newCommand.startsWith("duck")){// duck Move
        Serial.println("ducking move Started!");
        for(int i=0;i<8;i++){
          //Serial.println(Duck[i]);
          pwm.setPWM(i, 0, angleToPulse(Duck[i]));
        }
      }
      else if(newCommand.startsWith("T1")){
        Serial.println("Test01 move Started!");
        for(int i=0;i<8;i++){
          pwm.setPWM(i, 0, angleToPulse(Test01[i]));
          delay(Sdata);
        }
      }
      else if(newCommand.startsWith("T2")){
        Serial.println("Test02 move Started!");
        for(int i=0;i<8;i++){
          pwm.setPWM(i, 0, angleToPulse(Test02[i]));
          delay(Sdata);
        }
      }
      else if(newCommand.startsWith("AO")){//AutoOptimize
        //AutoOptimize(int move, int servoTarget, int stepSize = 1, int maxSteps = 30)
        // [0,3] servos são os da direita 
        // [4,7] servos são os da esquerda
        sscanf(newCommand.c_str(), "AO%d;%d;%d;%d", &Sdata, &Tdata, &Odata, &Ldata);
        Text = "Animation:"+String(Sdata)+"\nServo:    "+String(Tdata)+"\nstepSize: "+String(Odata)+"\nmaxSteps: "+String(Ldata);
        Serial.println(Text);
        Moves.AutoOptimize(Sdata, Tdata, Odata, Ldata);
      }
    }
  }
}