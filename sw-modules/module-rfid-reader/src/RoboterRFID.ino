#include <Arduino.h>
#include <MFRC522.h>

#define RFIDDETECTOR_SDA 5
#define RFIDDETECTOR_RST_PIN 6
#define RFID_LENGTH 7

MFRC522 mfrc522(RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN);   // Instanz des MFRC522 erzeugen

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("ready");
}

void loop()
{
  // Nur wenn eine Karte gefunden wird und gelesen werden konnte, wird der Inhalt von IF ausgeführt
  // PICC = proximity integrated circuit card = kontaktlose Chipkarte
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ) {
    Serial.println();
    Serial.print("Gelesene UID:");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      // Abstand zwischen HEX-Zahlen und führende Null bei Byte < 16
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }

  }

  // Versetzt die gelesene Karte in einen Ruhemodus, um nach anderen Karten suchen zu können.
  mfrc522.PICC_HaltA();
  delay(1000);

}
