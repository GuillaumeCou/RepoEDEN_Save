#include <SPI.h>
#include <LoRa.h>
byte STATE = LOW;
int temps;
void setup() {
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    digitalWrite(3, HIGH);
    temps = millis();
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  if( (millis() - temps) > 1000 ) {
    digitalWrite(3, LOW);
  }
}
