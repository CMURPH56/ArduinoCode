/*
 * 
 * All the resources for this project: https://www.hackster.io/Aritro
 * Modified by Aritro Mukherjee
 * 
 * 
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <Stepper.h>

// RFID Pins
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


// Motor Pins
int in1Pin = 5;
int in2Pin = 4;
int in3Pin = 3;
int in4Pin = 2;

const int stepsPerRevolution = 200; 


Stepper motor(stepsPerRevolution, in1Pin, in2Pin, in3Pin, in4Pin);

void setup() 
{
  // RFID setup
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  // Motor Setup
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  motor.setSpeed(60);

  
}
void loop() 
{
  checkAuth();
} 



void checkAuth()
{
   // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
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
  if (content.substring(1) == "FE 2D 91 79") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    rotateMotor(true);
  }
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }
}

void rotateMotor(bool runCode)
{
  while(runCode)
  {
        motor.step(stepsPerRevolution * 5);
        delay(500);
        motor.step(-stepsPerRevolution * 5);
        delay(500);
        runCode = false;
  }
}
