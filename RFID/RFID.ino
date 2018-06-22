

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
#include "HX711.h"  //You must have this library in your arduino library folder
 

#define DOUT  3 //pin Belegung Waage oberes Fach
#define CLK  2
#define DOUT_2  12//pin Belegung Waage unteres Fach
#define CLK_2 11
#define SS_PIN 53
#define RST_PIN 49
#define led_PIN 10


#define PinCheckOpenShelf 31

// define weight of tools to know which tool is in the shelf
#define zange 0.415
#define schraubenzieher 0.283
#define schere 0.028
#define gesamtgewicht1 2.079
#define bohrmaschine 0.956
#define hammer 0.386
#define bohraufsaetze 0.182
#define kiste 0.555
#define gesamtgewicht2 0.726
#define nullGewicht1 0.004


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

HX711 scale_1(DOUT, CLK);
HX711 scale_2(DOUT_2, CLK_2);
 
//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor1 = -98000; //-106600 worked for my 40Kg max scale setup 
float calibration_factor2 = -2146150;

float LastWeight1;
float LastWeight2;

float Objektliste1[] = {bohrmaschine, hammer, bohraufsaetze};
String objektnamen1[] = {"Bohrmaschine", "Hammer", "Bohraufsätze"};
float Objektliste2[] = {zange, schraubenzieher, schere};
String objektnamen2[] = {"Zange", "Schraubenzieher", "Schere"};
int val = 0; 
int schlossoffen = 0; 
float difference; // measures the difference between current weight and gesamtgewicht1/2
String fehlendesObj;

void setup() {


  //Setup Last weights
  LastWeight1= 2.079;
  LastWeight2= 0.726;
  lcd.begin(16,2);// Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
  //lcd.write("Linked Space FTW!");
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.println("Scan PICC to see UID and type...");
  pinMode ( led_PIN, OUTPUT);
  pinMode (PinCheckOpenShelf, INPUT);
  //digitalWrite(PinCheckOpenShelf, LOW);
  


  
  scale_1.set_scale(calibration_factor1);
  scale_2.set_scale(calibration_factor2);
//  scale_1.set_scale(calibration_factor1); //Adjust to this calibration factor
//  scale_2.set_scale(calibration_factor2); //Adjust to this calibration factor
  scale_1.tare();
  scale_2.tare();//Reset the scale to 0
  lcd.print("Linked Space FTW!");
 
  long zero_factor1 = scale_1.read_average(); //Get a baseline reading
  long zero_factor2 = scale_2.read_average();
  

  digitalWrite(led_PIN, HIGH);
}

bool intervalMeasure(float currentWeight, float referenceWeight ) {
  float h;
  h = referenceWeight + 0.10;
  float l;
  l = referenceWeight - 0.10;
  if(l < currentWeight and currentWeight < h)
    return true;
  else 
    return false;
}

void loop() {
  String currentUser ="";
  //digitalWrite(led_PIN, HIGH);
  scale_1.set_scale(calibration_factor1); //Adjust to this calibration factor
  scale_2.set_scale(calibration_factor2); //Adjust to this calibration factor
  Serial.println(scale_1.get_units(), 3);
  Serial.println(scale_2.get_units(), 3);
  delay(1000);

val= digitalRead(PinCheckOpenShelf);
if (val==1){
  Serial.println("Works");
}

  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }



if (mfrc522.uid.uidByte[0] == 0x04 &&
     mfrc522.uid.uidByte[1] == 0x6D &&
     mfrc522.uid.uidByte[2] == 0xF7 &&
     mfrc522.uid.uidByte[3] == 0xF2 &&
     mfrc522.uid.uidByte[4] == 0x46 &&
     mfrc522.uid.uidByte[5] == 0x4B &&
     mfrc522.uid.uidByte[6] == 0x80
     ) {
currentUser = "Mischa";

  }

if (mfrc522.uid.uidByte[0] == 0x69 &&
     mfrc522.uid.uidByte[1] == 0x8A &&
     mfrc522.uid.uidByte[2] == 0x6D &&
     mfrc522.uid.uidByte[3] == 0x37 
     ) {

currentUser="Fanny";

Serial.println("Hi " + currentUser);
delay(1000);
lcd.setCursor(0,0);
lcd.print("Hi " + currentUser);
delay(1000);
  }

if (mfrc522.uid.uidByte[0] == 0x56 &&
     mfrc522.uid.uidByte[1] == 0x24 &&
     mfrc522.uid.uidByte[2] == 0xC8 &&
     mfrc522.uid.uidByte[3] == 0x3B 
   ) {
  currentUser="Alex";
  }
//scale_1.get_units();



while(currentUser!="") {
 
 lcd.setCursor(0,0);
 Serial.println("Hi " + currentUser);
 delay(1000);
 lcd.print("Hi " + currentUser+"           ");
 delay(1000);
  digitalWrite(led_PIN, LOW);
  delay(3000);
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("xxxxxxxxxxxxxxxxxxxx");
  Serial.println("Tür offen");
  
  //ÄNDERUNG AUF PLATTE 1  
      // Look for new cards
      schlossoffen=digitalRead(led_PIN);
      while(schlossoffen == LOW){ // solange das Schloss offen ist, in der While schleife bleiben
        
          //Gewicht Kiste 1 ungleich dem zuletzt gemessenen Wert
          if( not(intervalMeasure(scale_1.get_units(), LastWeight1))) {
         
          Serial.println("Gewicht oberes Fach hat sich verändert");
          
              //Zurücklegen
              if (scale_1.get_units()>(LastWeight1+0.09)){
                    delay(500);
                    Serial.println("User möchte etwas zurückgeben");
                    delay(500);

                    //wenn Gewicht gleich dem ursprünglichen Gesamtgewicht entspricht, dann ist die Kiste vollständig --> passt
                    
                    if (intervalMeasure(scale_1.get_units(),gesamtgewicht1)==true){
                          Serial.println("Kiste vollständig, danke für die Rückgabe");
                          delay(1000);
                          LastWeight1 = scale_1.get_units();
                          delay(1000);
                          Serial.println("Bitte jetzt Tür schließen");
                          //lcd.setCursor(0,0);
                          //delay(1000);
                          //lcd.print("Danke für die Rückgabe");
                     }

                     //wenn Gewicht != ursprungsgewicht, dann fehlen Teile der Kiste --> Aufforderung gesamte Kiste zurückzugeben
                     
                    else {
                        Serial.println("Kiste unvollständig");
                        difference = gesamtgewicht1 - scale_1.get_units();
                        for( int i = 0; i <4 ; i++){
                          if (intervalMeasure(difference,Objektliste1[i]) == true){
                            fehlendesObj = Objektliste1[i];
                            Serial.println(objektnamen1[i] + "fehlt! Bitte nachlegen");
                            delay(500);
                            LastWeight1 = scale_1.get_units();
                            break;
                            //lcd.setCursor(0,0);
                            //lcd.print(fehlendesObj + " fehlt");
                      }
          
                     }
                    
                   }
        
               }

             //PLATTE 2: Box oder Tools ausleihen, wenn aktuelles Gewicht kleiner als letztes/ NUTZER MÖCHTE AUSLEIHEN
             
             if (scale_1.get_units()<(LastWeight2-0.09)){

                 Serial.println("User möchte ausleihen");

                  //wenn aktuelles Gewicht gleich 0 wurde komplette Kiste entnommen --> passt!
                  
                 if ( intervalMeasure(scale_1.get_units(),0)== true){
                  Serial.println("Komplette Kiste 1 wurde entnommen");
                  delay(1000);
                  LastWeight1= scale_1.get_units();
                  Serial.println("Bitte Tür jetzt schließen");
                  //lcd.setCursor(0,0);
                  //lcd.print("Kiste 1 entnommen");            
                 }

                 // wenn Gewicht nicht gleich null, dann wurde nicht die komplette Kiste entnommen --> Aufforderung gesamte Kiste zu entnehmen
                 else{
                  Serial.println("Bitte vollständige Kiste entnehmen");
                  delay(500);
                  LastWeight1= scale_1.get_units();
                  delay(1000);
                 // lcd.setCursor(0,0);
                 // lcd.print("Bitte gesamte Kiste entnehmen");
                 }
            
           }
      }

      //Gewicht Kiste2 ungleich dem zuletzt gemessenen Wert
          if( not(intervalMeasure(scale_2.get_units(), LastWeight2))) {
         
          Serial.println("Gwicht unteres Fach hat sich verändert");
          
              //Zurücklegen
              if (scale_2.get_units()>(LastWeight2+0.05)){
                    delay(500);
                    Serial.println("User gibt etwas zurückgeben");
                    delay(500);

                    //wenn Gewicht gleich dem ursprünglichen Gesamtgewicht entspricht, dann ist die Kiste vollständig --> passt
                    
                    if (intervalMeasure(scale_2.get_units(),gesamtgewicht2)==true){
                          Serial.println("Kiste 2 vollständig, danke für die Rückgabe");
                          delay(1000);
                          LastWeight2 = scale_2.get_units();
                          delay(1000);
                          Serial.println("Bitte jetzt Tür schließen");
                          break;
                          //lcd.setCursor(0,0);
                          //delay(1000);
                          //lcd.print("Danke für die Rückgabe");
                     }

                     //wenn Gewicht != ursprungsgewicht, dann fehlen Teile der Kiste --> Aufforderung gesamte Kiste zurückzugeben
                     
                    else {
                        Serial.println("Kiste unvollständig");
                        difference = gesamtgewicht2 - scale_2.get_units();
                        for( int i = 0; i <4 ; i++){
                          if (intervalMeasure(difference,Objektliste2[i]) == true){
                            fehlendesObj = Objektliste2[i];
                            Serial.println(objektnamen2[i] + "fehlt! Bitte nachlegen");
                            delay(500);
                            LastWeight2 = scale_2.get_units();
                            break;
                            //lcd.setCursor(0,0);
                            //lcd.print(fehlendesObj + " fehlt");
                      }
          
                     }
                    
                   }
        
               }

             //Box oder Tools ausleihen, wenn aktuelles Gewicht kleiner als letztes/ NUTZER MÖCHTE AUSLEIHEN
             
             if (scale_2.get_units()<(LastWeight2-0.05)){

                 Serial.println("User möchte etwas ausleihen");

                  //wenn aktuelles Gewicht gleich 0 wurde komplette Kiste entnommen --> passt!
                  
                 if ( intervalMeasure(scale_2.get_units(),0)== true){
                  Serial.println("Komplette Kiste 2 wurde entnommen");
                  lcd.setCursor(0,0);
                  lcd.print("Komplette Kiste 2 entnommen" );
                  delay(1000);
                  LastWeight1= scale_2.get_units();
                  Serial.println("Bitte Tür jetzt schließen");
                  //lcd.setCursor(0,0);
                  //lcd.print("Kiste 1 entnommen");            
                 }

                 // wenn Gewicht nicht gleich null, dann wurde nicht die komplette Kiste entnommen --> Aufforderung gesamte Kiste zu entnehmen
                 else{
                  Serial.println("Bitte vollständige Kiste entnehmen");
                  delay(500);
                  LastWeight2= scale_2.get_units();
                  delay(1000);
                  break;
                 // lcd.setCursor(0,0);
                 // lcd.print("Bitte gesamte Kiste entnehmen");
                 }
            
           }
      }
        if (  mfrc522.PICC_IsNewCardPresent()) {
        digitalWrite(led_PIN, HIGH);
        Serial.println("Auf Wiedersehen");
        delay(1000);
        lcd.setCursor(0,0);
       lcd.print("Ciao  " + currentUser );
       delay(3000);
       lcd.setCursor(0,0);
       lcd.print("                                ");
       //lcd.clear();
        currentUser="";
        LastWeight1= scale_1.get_units();
        LastWeight2= scale_2.get_units();
       return;
      }
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
      }

      
  //ÄNDERUNG AUF PLATTE 2    

  
}


 
}


