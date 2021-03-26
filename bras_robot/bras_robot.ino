D#include <Servo.h>
#include <SoftwareSerial.h>

Servo base;
Servo bras;
Servo avant_bras;
Servo poignet;

SoftwareSerial BTSerial(11,12);

boolean newData = false;
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
char messageFromAndroid[numChars] = {0};
int integerFromAndroid = 0;


/*
 * Pour chaque servo, on va commencer par lire la valeur de la tension sur le potentiometre, le convertir en degres avec map, puis l'ecrire sur le servo moteur
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Begin, setting up");
  pinMode(6, INPUT);
  BTSerial.begin(9600);
  Serial.println("Bluetooth starting at 9600");
  
  base.attach(4);
  bras.attach(5);
  avant_bras.attach(3);
  poignet.attach(2);
  
}


void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;  //Determine if the start marker has been received or not
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (BTSerial.available() > 0 && newData == false) {
        rc = BTSerial.read();

        if (recvInProgress == true) {s
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void parseData(){
    char * strtokIndx; // this is used by strtok() as an index
    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromAndroid, strtokIndx); // copy it to messageFromAndroid
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromAndroid = atoi(strtokIndx);     // convert this part to an integer

}

void servoCommand(char * servo, int angle){
  if (strcmp(servo, "BASE") == 0){
    base.write(angle);
  }
  else if (strcmp(servo, "ARM") == 0){
    bras.write(angle);
  }
  else if (strcmp(servo, "FOREARM") == 0){
    avant_bras.write(angle);
  }
  else if (strcmp(servo, "WRIST") == 0){
    poignet.write(angle);
  }
  else {
    Serial.println("Error in servo name, check the data parsed");
  }
}

void loop() {
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    servoCommand(messageFromAndroid, integerFromAndroid);
    newData = false;
  }
}
