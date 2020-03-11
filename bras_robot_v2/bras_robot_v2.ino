#include <Servo.h>
Servo base;
Servo bras;
Servo avant_bras;
Servo poignet;

//Position pour les angles des servos
const int pos_min = 650;
const int pos_max = 2400;


//Valeur des angles, alpha est l'angle du servo du bras, beta l'angle de servo avant bras

double alpha;
double beta;

//Prochain angle du servo
double angleServo1;
double angleServo2;

//La position voulue, 1er element est x, 2nde est z. Distance maximale atteignable est d = longueur * sqrt(2)
double target[2];

//La valeur de l'angle precedant la lecture 
double previousAlpha = 0;
double previousBeta = 0;

//Declaration des potentiometres
const int pot1 = A1;
const int pot2 = A2;


//Position du servo 2
double pos_Servo2[2];

//Longeur des bras, c la distance servo1 -> target et la hauteur du triangle (voir schéma)
float longueur = 15.5;
double c;
double hauteur;

//Declaration des millis
unsigned long CurrentMillis = 0;
unsigned long ServoMillis = 0;

//Frequence d'echantillonnage
int updateInterval = 500;

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Begin, setting up");
  
  base.attach(5);
  bras.attach(6);
  avant_bras.attach(7);
  poignet.attach(8);
  bras.write(0);
  avant_bras.write(0);


}

void loop() {
  // put your main code here, to run repeatedly:
  CurrentMillis = millis();
  target[0] = mesure(pot1);
  target[1] = mesure(pot2);
  
  c = sqrt(pow(target[0],2) + pow(target[1],2));
  hauteur = sqrt(pow(longueur,2) + pow(c/2, 2));

  Serial.write("C= ");
  Serial.println(c);
  
  writeBras();
  writeAvantBras();
}
void calcul_alpha(){
  double alpha1;
  double alpha2;
  float b = c/2;

  //Les valeurs de retours sont données en radians, convertis en degres
  alpha2 = atan(target[1]/target[0]) * 180.0 / PI;
  alpha1 = acos(b/longueur) * 180.0 / PI ;
  alpha = alpha1 + alpha2;
  Serial.print("Alpha: ");
  Serial.println(alpha);

}

void calcul_posServo2(){
  pos_Servo2[0] = cos(alpha)*longueur;
  pos_Servo2[1] = sin(alpha)*longueur;
}

void calcul_beta(){
  calcul_posServo2();
  beta = atan((target[0] - pos_Servo2[0])/target[1] - pos_Servo2[1]);
  beta = beta * (180 / PI);
  
  if (beta <= 0){
    beta = 90 + beta;
  }
  
  Serial.write("Beta: ");
  Serial.println(beta);
}

void writeBras(){
  calcul_alpha();
  
  if (alpha != previousAlpha && (CurrentMillis - ServoMillis) > updateInterval){
    ServoMillis = CurrentMillis;

    previousAlpha = alpha;

    bras.write(alpha);  
    
  }
}

void writeAvantBras(){
    calcul_beta();
  if (beta != previousBeta && (CurrentMillis - ServoMillis) > updateInterval){
    ServoMillis = CurrentMillis;
    previousBeta = beta;
    avant_bras.write(beta); 
  }
}


int mesure(int pin){
  int valpin;
  float mesure;
  
  mesure = analogRead(pin);
  //Convertit la valeur mesuree entre 0 et 1023 en une autre valeur entre 0 et 180
  valpin = map(mesure,0,1023,0,31);
  Serial.println(valpin);
  return valpin;
}
