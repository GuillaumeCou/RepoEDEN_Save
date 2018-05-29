#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>

const byte LEDPIN = 4;
const byte INTERRUPTPIN = 3;
volatile byte STATE = LOW;
const int taille_msg = 26;

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

void envoyer() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("Alert receive");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
}

void blink() {
  STATE = !STATE;
}

void alarme_BT() {
  mySerial.println("Alerte");
}

void loop() {
  digitalWrite(LEDPIN, STATE)
  if(STATE)
  {
    alarme_BT();
    envoyer();
    
  }
}
