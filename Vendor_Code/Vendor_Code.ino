#include <SPI.h>
#include <MFRC522.h>
#include<Servo.h>
#include <QueueArray.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define servoPin1 3
#define servoPin2 5
#define BUTTON_PIN1 2
#define BUTTON_PIN2 4
#define BUTTON_PIN3 6

Servo s1;
Servo s2;
MFRC522 mfrc522(SS_PIN, RST_PIN);   

unsigned long prev = 0;
unsigned long cur = 0;

QueueArray<int>q;
int number = 1;
int time = 0;
 
void setup() 
{
  Serial.begin(9600);   
  SPI.begin();      
  mfrc522.PCD_Init(); 
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);
  pinMode(7, OUTPUT);
  s1.attach(servoPin1);
  s2.attach(servoPin2);
  s1.write(0);
  s2.write(0);  
  Serial.println("Scan Your Card Please.......");
  Serial.println();
}

void loop() 
{

  cur = millis();

  if(cur-prev>=1000){
    if(!q.isEmpty ()){
      time++;
      digitalWrite(7,HIGH);
    }
    if(time==5){
      int a = q.pop();
      Serial.println(a);
      time = 0;
      digitalWrite(7,LOW);
    }
    prev=cur;
  }

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "63 00 47 A8") 
  {
    Serial.println("Authorized access....Select Your Option....");

    int r = 0;

    while(r!=1){
      byte buttonState1 = digitalRead(BUTTON_PIN1);
      byte buttonState2 = digitalRead(BUTTON_PIN2);
      byte buttonState3 = digitalRead(BUTTON_PIN3);
      int push1 = 0;
      int push2 = 0;
      
      if (buttonState1 == LOW ) {
        Serial.println("Box 1 is selected....");
        push1 = 1;
        if(push1==1){
          for(int i = 0;i<=90;i++){
            s1.write(i);
            delay(15);
          }
          delay(5000);
          for(int i = 90;i>=0;i--){
            s1.write(i);
            delay(15);
          }
          push1 = 0;
          r = 1;
          Serial.println("Scan Your Card Please.......");
        }      
      }else if(buttonState2 == LOW ) {
        Serial.println("Box 2 is selected....");
        push2 = 1;
        if(push2==1){
          for(int i = 0;i<=90;i++){
            s2.write(i);
            delay(15);
          }
          delay(5000);
          for(int i = 90;i>=0;i--){
            s2.write(i);
            delay(15);
          }
          push2 = 0;
          r = 1;  
          Serial.println("Scan Your Card Please......."); 
        }             
      }else if(buttonState3 == LOW ) {
        Serial.println("Entered your id in Queue....");
        Serial.println(number);
        q.enqueue (number);
        number++;
        r = 1;  
        Serial.println("Scan Your Card Please.......");             
      }else {
        //Serial.println("Button is not pressed");
      }
      //delay(200);    
      }   
   // delay(500);
  }else{
    Serial.println("Access denied");
    //delay(500);
  } 
  
}

