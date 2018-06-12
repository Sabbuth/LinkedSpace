

/*
 * MFRC522 - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT.
 * The library file MFRC522.h has a wealth of useful info. Please read it.
 * The functions are documented in MFRC522.cpp.
 *
 * Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
 * Created by Miguel Balboa (circuitito.com), Jan, 2012.
 * Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013 (Translation to English, refactored, comments, anti collision, cascade levels.)
 * Released into the public domain.
 *
 * Sample program showing how to read data from a PICC using a MFRC522 reader on the Arduino SPI interface.
 *----------------------------------------------------------------------------- empty_skull 
 * Aggiunti pin per arduino Mega
 * add pin configuration for arduino mega
 * http://mac86project.altervista.org/
 ----------------------------------------------------------------------------- Nicola Coppola
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               51                MOSI
 * SPI MISO   12               50                MISO
 * SPI SCK    13               52                SCK
 *
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com. 
 */

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>


#define SS_PIN 53
#define RST_PIN 49
#define led_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

void setup() {
  lcd.begin(16,2);// Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
  //lcd.write("hello world!");
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.println("Scan PICC to see UID and type...");
  pinMode ( led_PIN, OUTPUT);
}

void loop() {

  digitalWrite(led_PIN, HIGH);
  

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  
  // Dump debug info about the card. PICC_HaltA() is automatically called.
 mfrc522.PICC_DumpToSerial(&(mfrc522.uid));


if (mfrc522.uid.uidByte[0] == 0x04 &&
     mfrc522.uid.uidByte[1] == 0x6D &&
     mfrc522.uid.uidByte[2] == 0xF7 &&
     mfrc522.uid.uidByte[3] == 0xF2 &&
     mfrc522.uid.uidByte[4] == 0x46 &&
     mfrc522.uid.uidByte[5] == 0x4B &&
     mfrc522.uid.uidByte[6] == 0x80
     ) {

Serial.println("Hi Mischa");
lcd.clear();
//lcd.blink();
delay(500);
lcd.print("Hi Mischa");
delay(500);

digitalWrite(led_PIN, LOW);
  delay(10000);
  digitalWrite(led_PIN, HIGH);
  Serial.println("Been here, done this...");
  }

if (mfrc522.uid.uidByte[0] == 0x69 &&
     mfrc522.uid.uidByte[1] == 0x8A &&
     mfrc522.uid.uidByte[2] == 0x6D &&
     mfrc522.uid.uidByte[3] == 0x37 
     ) {

Serial.println("Hi Fanny");
lcd.clear();
//lcd.blink();
delay(500);
lcd.print("Hi Fanny");
delay(500);

digitalWrite(led_PIN, LOW);
  delay(10000);
  digitalWrite(led_PIN, HIGH);
  Serial.println("Been here, done this...");
  }

if (mfrc522.uid.uidByte[0] == 0x56 &&
     mfrc522.uid.uidByte[1] == 0x24 &&
     mfrc522.uid.uidByte[2] == 0xC8 &&
     mfrc522.uid.uidByte[3] == 0x3B 
   ) {
Serial.println("Hi Alex");
lcd.clear();
//lcd.blink();
delay(500);
lcd.write("Hi Alex");
delay(500);

digitalWrite(led_PIN, LOW);
  delay(10000);
  digitalWrite(led_PIN, HIGH);
  Serial.println("Been here, done this...");
  }
 
}


