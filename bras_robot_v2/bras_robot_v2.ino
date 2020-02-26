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

//La position voulue, 1er element est x, 2nde est z
double target[2];

//Position du servo 2
double pos_Servo2[2];

float longueur = 15.5;
double c = sqrt(pow(target[0],2) + pow(target[1],2));
double hauteur = sqrt(pow(longueur,2) + pow(c/2, 2));


void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Begin, setting up");
  
  base.attach(5);
  bras.attach(6);
  avant_bras.attach(7);
  poignet.attach(8);


}

void loop() {
  // put your main code here, to run repeatedly:
  calcul_alpha();
  calcul_posServo2();
  calcul_beta();

  if (alpha < beta) {
    for (int i = 0; i <= beta; i ++){
      if (i <= alpha){
        avant_bras.write(i);
      }
      bras.write(i);
      delay(50);
    }
  }
  else{
    for (int i = 0; i <= alpha; i ++){
      if (i <= beta){
        avant_bras.write(i);
      }
    bras.write(i);
    delay(50);
    }
  }

  avant_bras.write(beta);
}

void calcul_alpha(){
  double alpha1;
  double alpha2;
  float b = c/2;

  //Les valeurs de retours sont données en radians, convertis en degres
  alpha2 = atan(target[1]/target[0]) * 180.0 / PI;
  alpha1 = acos(b/longueur) * 180.0 / PI ;
  
  alpha = alpha1 + alpha2;
  
}

void calcul_posServo2(){
  pos_Servo2[0] = cos(alpha)*longueur;
  pos_Servo2[1] = sin(alpha)*longueur;
}

void calcul_beta(){
  beta = atan((target[0] - pos_Servo2[0])/target[1] - pos_Servo2[1]);

  beta = beta * (180 / PI);
  
  if (target[1] - pos_Servo2[2]){
    beta = beta - 180;
  }
}

void coordination(double angle_bras, double angle_avBras){
  
}
