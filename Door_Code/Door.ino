#include <Servo.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

#define SERVO_PIN 1
#define LED_PIN 2

Servo servo;
int flag = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);  // set the LED pin as an output
  servo.attach(SERVO_PIN);   // attach the servo to its pin
  servo.write(0);            // set the initial position of the servo
  SPI.begin();               // start the SPI communication
  mfrc522.PCD_Init();        // initialize the RFID scanner
}

void loop() {

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.println("UID: " + uid);

    if (uid == "27ad2cf") {

      Serial.println("Access granted");

      if (flag == 0) {
        flag = 1;
        servo.write(90);
        digitalWrite(LED_PIN, HIGH);
      } else {
        flag = 0;
        servo.write(0);
        digitalWrite(LED_PIN, LOW);
        delay(1000);
      }

    } else {
      Serial.println("Access denied");
      delay(2000);
    }
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
