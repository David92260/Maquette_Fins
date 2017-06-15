/* 
 Nom du projet:  Maquette de déploiement Callisto  
 Auteur:         David Monchaux 
 Company:        CNES 
 Description:    Programme Arduino pour le contrôle du déploiement des fins 
 Date : 14 juin 2017
 */ 


#include <Servo.h> 
 

 const int stepsPerRevolution = 200;   // Valeur à régler en fonction du moteur choisi (nombre de steps par tour) 
 

 // initialize the stepper library on pins 8 through 11: 
 Servo myServo1; 
 Servo myServo2; 
 Servo myServo3; 
 Servo myServo4; 

 //Declare pin functions on Redboard
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6

// Broche 7 en sortie commande du relais
const int RELAIS = 10; //declaration broche 10 output

// Etat du relais (on / off)


//Declare variables for stepper functions
char user_input;
int x;
int y;
int state;


 int stepCount = 0;  // number of steps the motor has taken 
 int cases=0;           //start condition to print the menu.  
 
int  etat_relais = 48;
 void Stepper_Control(); 
 void Test_pilotage_fins(); 
 void resetEDPins();

 void setup(){ 
   //Set baudrate to 9600 for serial communication 
   Serial.begin(9600); 
 

   //set pin I/O 
 
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(8,OUTPUT); 
  myServo1.attach(8); 
   pinMode(RELAIS,OUTPUT); 
//  pinMode(9,OUTPUT); myServo2.attach(9); myServo2.write(90); 
//  pinMode(10,OUTPUT); myServo3.attach(10); myServo3.write(90); 
//  pinMode(11,OUTPUT); myServo4.attach(11); myServo4.write(90); 
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  
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
     Stepper_Control(); 
     resetEDPins();
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
 

 void Stepper_Control()
 { 
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
 

 Serial.println("Alternate between stepping forward and reverse.");
 //digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
  for(x= 1; x<5; x++)  //Loop the forward stepping enough times for motion to be visible
  {
     digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
    //Read direction pin state and change it
    state=digitalRead(dir);
    if(state == HIGH)
    {
      digitalWrite(dir, LOW);
    }
    else if(state ==LOW)
    {
      digitalWrite(dir,HIGH);
   
    }

    for(y=1; y<1000; y++)
    {
      digitalWrite(stp,HIGH); //Trigger one step
      delay(1);
           // Serial.println("j'ai boucle 1000 fois");
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(1);
    }
 
  }
  //    digitalWrite(EN, HIGH);

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
 
 

 void Test_pilotage_fins(){ 
 
   int temp; 
   while(1){ 
     Serial.print("\n----------------------------------\n"); 
     Serial.print("\nTest du pilotage des fins...      \n"); 
     Serial.print("\n----------------------------------\n"); 
     Serial.print("\nAssurez-vous que les fins sont bien deployes, et que rien n'entrave le mouvement des volets\n"); 
     Serial.print("\nEntrez 1 pour lancer la procedure.\n"); 
     while(1){ 
       temp = retrieve_data(); 
       if(temp == 49){   
         break; 
       } 
       else Serial.print("Entrez 1 pour lancer la procedure.\n"); 
     }; 


     /* Test de pilotage des ailerons*/ 
     /* Etat relais*/

     digitalWrite(RELAIS,HIGH); // Ferme le relais
     delay(10);
     myServo1.write(60); // mettre le servo à 60° => déplacement de -30° à partir de la position centrale à 90° 
     delay(1000); // ce délai est nécessaire pour que le servo atteigne sa position 
     myServo1.write(120); // position à +30° par rapport à la position centrale à 90° 
     delay(1000); // attente à nouveau 
     myServo1.write(90); // remettre le servo à 90°, position centrale 
     delay(1000);
     digitalWrite(RELAIS,LOW); // Ferme le relais
     
//     myServo2.write(60); // mettre le servo à 60° => déplacement de -30° à partir de la position centrale à 90° 
//     delay(1000); // ce délai est nécessaire pour que le servo atteigne sa position 
//     myServo2.write(120); // position à +30° par rapport à la position centrale à 90° 
//     delay(1000); // attente à nouveau 
//     myServo2.write(90); // remettre le servo à 90°, position centrale 
// 
//
//     myServo3.write(60); // mettre le servo à 60° => déplacement de -30° à partir de la position centrale à 90° 
//     delay(1000); // ce délai est nécessaire pour que le servo atteigne sa position 
//     myServo3.write(120); // position à +30° par rapport à la position centrale à 90° 
//     delay(1000); // attente à nouveau 
//     myServo3.write(90); // remettre le servo à 90°, position centrale 
// 
//
//     myServo4.write(60); // mettre le servo à 60° => déplacement de -30° à partir de la position centrale à 90° 
//     delay(1000); // ce délai est nécessaire pour que le servo atteigne sa position 
//     myServo4.write(120); // position à +30° par rapport à la position centrale à 90° 
//     delay(1000); // attente à nouveau 
//     myServo4.write(90); // remettre le servo à 90°, position centrale 
//      
     Serial.print("\nLes quatre ailerons ont-ils bouge?\n1. Oui.\n2. Non, recommencer le test.\n"); 
     while(1){ 
       temp = retrieve_data();              //wait for answer 
       if (temp == 50 | temp == 49){       //is it a 1 or 2? 
         break; 
       } 
       else Serial.print("Entrez 1 ou 2.\n"); 
     } 
     if (temp == 49){                        //if '1' is received, end this case. 
       break; 
     }                             //if '2' is reveived, the case will start again. 
   } 
 } 

//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}
