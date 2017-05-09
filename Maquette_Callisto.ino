/*
 Nom du projet:  Maquette de déploiement Callisto 
 Auteur:         David Monchaux
 Company:        CNES
 Description:    Programme Arduino pour le contrôle du déploiement des fins
 */

#include <Stepper.h>
#include <Servo.h>

const int stepsPerRevolution = 200;   // Valeur à régler en fonction du moteur choisi (nombre de steps par tour)

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myServo4;

int stepCount = 0;  // number of steps the motor has taken
int cases=0;           //start condition to print the menu. 
int motorSpeed = 50;  //vitesse moteur initialisée à 50%

void Stepper_Control(int VitesseMoteur);
void Test_pilotage_fins();

void setup(){
  //Set baudrate to 9600 for serial communication
  Serial.begin(9600);

  //set pin I/O

  pinMode(1,OUTPUT); myServo1.attach(1); myServo1.write(90);  // initialize and set servo to mid-point
  pinMode(2,OUTPUT); myServo2.attach(2); myServo2.write(90);
  pinMode(3,OUTPUT); myServo3.attach(3); myServo3.write(90);
  pinMode(4,OUTPUT); myServo4.attach(4); myServo4.write(90);
  
}

void loop(){

  switch (cases){

  case 0:                                                            //start condition   
    Serial.begin(9600);
    Serial.print("\n\n\nBienvenue !\nCette interface permet de controler la maquette CALLISTO.\n");  
    Serial.print("\nChoisir une des options ci-dessous:\n\n");
    Serial.print("1. Deploiement des fins\n");
    Serial.print("2. Test du pilotage des fins\n");
    cases = retrieve_data();
    break;

  case 10:                                                          //new line sent by pc
    cases = retrieve_data();
    break;

  case 49:                                                          //caracter 1 sent by pc
    Stepper_Control(motorSpeed);
    cases=0;
    break;

  case 50:                                                          //character 2 sent by pc
    Test_pilotage_fins();                                           
    cases=0;
    break;

  default:                                                          //if wrong number received                                            
    Serial.print("The robot has received an invalid number, please enter 1 - 4\n");
    cases = retrieve_data();
    break;
  }   
}

int retrieve_data(){                                    //reads the data that is sent as last
  int inputByte;
  int counter;
  int availablebytes;
  while(1){
    if ( Serial.available() > 0){                      // any data waiting?
      break;                                         // if not, stop function.
    }
  }
  availablebytes = Serial.available();                 // if data available, read the data.
  for(counter=0;counter < availablebytes;counter++){    
    inputByte = Serial.read();                    // make sure the last data in the buffer is used. 
  };                                               // recommend only to use this method in this selftest.
  return inputByte;
}

void Stepper_Control(int VitesseMoteur){ //VitesseMoteur est un entier entre 0 et 100
  int temp;
  while(1){
    Serial.print("\n----------------------------------\n");
    Serial.print("\nAttention, deploiement des fins...\n");
    Serial.print("\n----------------------------------\n");
    Serial.print("\nAssurez-vous que rien n'entrave le deploiement. Entrez 1 pour lancer la procedure.\n");
    while(1){
      temp = retrieve_data();
      if(temp == 49){  
        break;
      }
      else Serial.print("Entrez 1 pour lancer la procedure.\n");
    };  

    // set the motor speed:
    if (VitesseMoteur > 0 && VitesseMoteur <= 100) {
    myStepper.setSpeed(VitesseMoteur);

    // Faire 1 tour par incrément de 1/100 de tour:
    for (int i=1; i <= 100; i++){ 
      myStepper.step(stepsPerRevolution / 100);
      delay(20);
    }

    Serial.print("\nFin du deploiement, entrez 1 pour terminer.\n");
    while(1){
      temp = retrieve_data();              //wait for answer
      if (temp == 49){ 
        break;
      }
      else Serial.print("Entrez 1 pour terminer.\n");
    }
    if (temp == 49){                        //if '1' is received, end this case.
      break;
    }
  }
}
}

void Test_pilotage_fins(){


  int temp;
  while(1){
    Serial.print("\nWelcome to the motor test.\nPlease be sure that the robot CANNOT MOVE\nThe weels must be off the floor when you start the test.\n\n1. Start the motor test.\n");
    while(1){
      temp = retrieve_data();
      if(temp == 49){  
        break;
      }
      else Serial.print("Please enter 1 to start the test.\n");
    };
    /* Test de pilotage des ailerons*/

    myServo1.write(60); // mettre le servo à 60° => déplacement de -30° à partir de la position centrale à 90°
    delay(1000); // ce délai est nécessaire pour que le servo atteigne sa position
    myServo1.write(120); // position à +30° par rapport à la position centrale à 90°
    delay(1000); // attente à nouveau
    myServo1.write(90); // remettre le servo à 90°, position centrale
    
    Serial.print("\nDid both motors work?\n1. Yes It's fine.\n2. No, run the test again. \n");
    while(1){
      temp = retrieve_data();              //wait for answer
      if (temp == 50 | temp == 49){       //is it a 1 or 2?
        break;
      }
      else Serial.print("Please enter 1 or 2\n");
    }
    if (temp == 49){                        //if '1' is received, end this case.
      break;
    }                             //if '2' is reveived, the case will start again.
  }
}






