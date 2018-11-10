#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 4         //D2
#define RST_PIN 5        //D1

MFRC522 mfrc522(SS_PIN,RST_PIN);

//int statuss = 0;
//int out = 0;

void setup(){
  Serial.begin(9600);
  SPI.begin();          //Initiates SPI connection between RFID module and Arduino
  mfrc522.PCD_Init();   //Initiates MFRC522 RFID module
}

void loop(){
  //Look for cards
  if (!mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  //Select one of the cards
  if(!mfrc522.PICC_ReadCardSerial()){
    return;
  }

  String content = "";
  byte letter;
  for(byte i = 0; i < mfrc522.uid.size; i++){
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  
}
