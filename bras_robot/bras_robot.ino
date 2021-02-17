#include <Servo.h>
Servo base;
Servo bras;
Servo avant_bras;
Servo poignet;

const int pot1 = A1;
int val1;
const int pot2 = A2;
int val2;
const int pot3 = A3;
int val3;
const int pot4 = A4;
int val4;

/*
 * Pour chaque servo, on va commencer par lire la valeur de la tension sur le potentiometre, le convertir en degres avec map, puis l'ecrire sur le servo moteur
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Begin, setting up");

  pinMode(pot1 , INPUT);
  pinMode(pot2, INPUT);
  pinMode(pot3, INPUT);
  pinMode(pot4, INPUT);
  
  base.attach(5);
  bras.attach(6);
  avant_bras.attach(7);
  poignet.attach(8);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  val1 = mesure(pot1);
  val2 = mesure(pot2);
  val3 = mesure(pot3);
  val4 = mesure(pot4);
  base.write(val1);
  bras.write(val2);
  avant_bras.write(val3);
  poignet.write(val4);
}

//Cette fonction mesure la valeur de la tension sur le potentiometre
int mesure(int pin){
  int valpin;
  float mesure;
  
  mesure = analogRead(pin);
  //Convertit la valeur mesuree entre 0 et 1023 en une autre valeur entre 0 et 180
  valpin = map(mesure,0,1023,0,180);
  Serial.println(valpin);
  return valpin;
  
}
