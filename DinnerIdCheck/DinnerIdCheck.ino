/*
    dinnerId.csv 양식
          
    ID,tag0,tag1,tag2,tag3\n
    1101,245,123,256,764\n
    2422,3,43,201,75\n
    1102,224,164,194,299\n
    1103,333,558,192,168\n
*/

#include <CSV_Parser.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <MFRC522.h>
#include <deprecated.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <LiquidCrystal_I2C.h>

const int cs0 = 8;  //cs for sd

const int cs1 = 10;  //cs for rfid
const int rstPin = 9;

int dinRegister = 3;  //1-석식 신청, 0-신청 안함

byte nuidPICC[4];

int row = 0;

int RLed = 3;
int GLed = 4;
int buzPin = 5;

CSV_Parser cp(/*format*/ "LLLLL", /*has_header*/ true, /*delimiter*/ ',');
MFRC522 mfrc(cs1, rstPin);
LiquidCrystal_I2C lcd(0x27, 14, 2);

void Ydin();  //신청 o
void Ndin();  //신청 x

int32_t *ID ;  //학번
int32_t *tag0;  //111
int32_t *tag1;  //222
int32_t *tag2;  //333
int32_t *tag3;  //444

void setup() {
    Serial.begin(9600);
    SPI.begin();
    Wire.begin();
    lcd.init();
    lcd.backlight();
    mfrc.PCD_Init();

    pinMode(RLed, OUTPUT);
    pinMode(GLed, OUTPUT);

    lcd.setCursor(2, 0);
    lcd.print(F("Initializing"));
    lcd.setCursor(3, 1);
    lcd.print(F("SD card..."));
    delay(1000);
    lcd.clear();
  
     // see if the card is present and can be initialized:
    if (!SD.begin(cs0)) {
        lcd.setCursor(4, 0);
        Serial.print(F("SD Failed"));
        while (1);
    }
    
    if (cp.readSDfile("/dinnerId.csv")) {  //dinnerId.csv라는 파일 읽기
        lcd.setCursor(1, 0);
        lcd.print(F("Analyzing file"));
        lcd.setCursor(2, 1);
        lcd.print(F("Please wait"));
        delay(1000);
        lcd.clear();
        
        ID =(int32_t*)cp["ID"];  //학번
        tag0 = (int32_t*)cp["tag0"];  //111
        tag1 = (int32_t*)cp["tag1"];  //222
        tag2 = (int32_t*)cp["tag2"];  //333
        tag3 = (int32_t*)cp["tag3"];  //444

        lcd.setCursor(2, 0);
        lcd.print(F("Ready to use"));
        delay(1000);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print(F("Scan ID Card"));
        
        if (ID && tag0 && tag1 && tag2 && tag3) {  //csv parser test
            return;
        } else {
            lcd.setCursor(1, 0);
            lcd.print(F("ERROR OCCURRED"));
        }
    } else {
        lcd.setCursor(1, 0);
        lcd.print(F("File not found"));
    }
}

void loop() {
    if (!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial()) {
        return;
    }

    if ((mfrc.uid.uidByte[0] != nuidPICC[0]) || (mfrc.uid.uidByte[1] != nuidPICC[1]) || (mfrc.uid.uidByte[2] != nuidPICC[2]) || (mfrc.uid.uidByte[3] != nuidPICC[3])) {
        for (byte i = 0; i < 4; i++) {
            nuidPICC[i] = mfrc.uid.uidByte[i];
        }
    
        for(row = 0; row < cp.getRowsCount(); row++) {
            if ((mfrc.uid.uidByte[0]==tag0[row]) && (mfrc.uid.uidByte[1]==tag1[row]) && (mfrc.uid.uidByte[2]==tag2[row]) && (mfrc.uid.uidByte[3]==tag3[row])) {
                dinRegister = 1;
                break;
            } else {
                dinRegister = 0;
            }
        }
    
        if (dinRegister == 1) {
            Ydin();
        } else if (dinRegister == 0) {
            Ndin();
        } else {
            return;
        }
    } else {
        return;
    }
}

void Ydin() {
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print(ID[row]);
    lcd.setCursor(3, 1);
    lcd.print(F("Signed up"));
    digitalWrite(GLed, HIGH);
    tone(buzPin, 3951, 100);
    delay(900);
    digitalWrite(GLed, LOW);
    lcd.clear();

    lcd.setCursor(2, 0);
    lcd.print(F("Scan ID Card"));
}

void Ndin() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Didn't signed up"));
    digitalWrite(RLed, HIGH);
    tone(buzPin, 3729);
    delay(175);
    noTone(buzPin);
    delay(100);
    tone(buzPin, 3729);
    delay(175);
    noTone(buzPin);
    delay(100);
    tone(buzPin, 3729);
    delay(175);
    noTone(buzPin);
    delay(100);
//    tone(buzPin, 3729);
//    delay(175);
//    noTone(buzPin);
//    delay(100);
    digitalWrite(RLed, LOW);
    lcd.clear();

    lcd.setCursor(2, 0);
    lcd.print(F("Scan ID Card"));
}
