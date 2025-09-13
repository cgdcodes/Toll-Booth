#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo servoMotor;  // Create servo instance

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522

  servoMotor.attach(8); // Attach servo to pin 8
}

void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("RFID tag detected!");

    // Show UID on serial monitor
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
    if (content.substring(1) == "YOUR_TAG_UID") {
      Serial.println("Authorized access");
      servoMotor.write(90);  // Rotate servo to 90 degrees
      delay(1000);           // Wait for 1 second
      servoMotor.write(0);   // Rotate servo to 0 degrees
      delay(1000);           // Wait for 1 second
    } else {
      Serial.println("Access denied");
    }
  }
  mfrc522.PICC_HaltA();  // Stop reading
  mfrc522.PCD_StopCrypto1(); // Stop encryption
}
