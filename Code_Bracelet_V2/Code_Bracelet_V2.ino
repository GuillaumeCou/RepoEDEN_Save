#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>

//const byte LEDPIN = 4;
const byte INTERRUPTPIN = 3;
volatile byte STATE = LOW;
const int taille_msg = 26;
char MESSAGE[taille_msg] = "180197Guillaume";

int counter = 0;
const int temps_attente_BT = 30;

SoftwareSerial mySerial(5, 4); // RX //  TX 
// Connect HM10      Arduino Uno
//     TXD          Pin 0
//     RXD          Pin 1


void setup() {
  // DEL & interruption
  Serial.begin(9600);
  //pinMode(LEDPIN, OUTPUT);
  pinMode(INTERRUPTPIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), blink, RISING);

  // LoRa
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Bluetooth
  Serial.begin(9600);
  mySerial.begin(9600);

  //digitalWrite(LEDPIN, STATE);
}

void envoyer(char tab[]) {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print(tab);
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
}

bool testco2() {
  int r = 0;
  bool test = false;
  
  //Serial.println(t);
  if(mySerial.available()){
    r = mySerial.read();
  }
  
  if (r != 0 ){
    test = true;
  }
  
  return test;
}

void blink() {
  STATE = !STATE;
}

void alarme_BT() {
  mySerial.println("Declencher l alarme");
}

void modification() {
  int i = 0;
  char newMessage[taille_msg];
  while(i<taille_msg)
  {
    char c;
    c = mySerial.read();
    newMessage[i] = c;
    i++;
  }
  *MESSAGE = newMessage;
}

void alerte() {
  if(testco2())
  {
    alarme_BT();
  }
  else
  {
    envoyer(MESSAGE);
  }
}

void loop() {
  if(STATE)
  {
    alerte();
  }
  
  if(mySerial.available()){
    int i = 0;
    char newMessage[taille_msg];
    while(mySerial.available() && i < taille_msg)
    {
      char c;
      c = mySerial.read();
      newMessage[i] = c;
      i++;
    }
    *MESSAGE = newMessage;

    Serial.println(MESSAGE);
  }

}
