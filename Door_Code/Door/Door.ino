#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define servoPin 3
#define LED_PIN 2


MFRC522 mfrc522(SS_PIN, RST_PIN);

Servo servo;

int flag = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);
  servo.attach(servoPin);
  servo.write(0);
  Serial.println("Scan Your Card Please.......");
  Serial.println();  // initialize the RFID scanner
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "02 7A D2 CF") {
    Serial.println("Authorized access....Door is opening.........");
    
    if (flag == 0) {
      flag = 1;
      for (int i = 0; i <= 90; i++) {
        servo.write(i);
        delay(15);
      }
      digitalWrite(LED_PIN, HIGH);
      delay(100);
    }else{
      flag = 0;
      for (int i = 90; i >= 0; i--) {
        servo.write(i);
        delay(15);
      }
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  } else {
    Serial.println("Access denied");
    delay(500);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

}
