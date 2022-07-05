/*
      column_1,column_2\n
      1,2\n
      11,22\n
      111,333\n
*/

#include <CSV_Parser.h>
#include <SPI.h>
#include <SD.h>

CSV_Parser cp(/*format*/ "LLLLL", /*has_header*/ true, /*delimiter*/ ',');

const int cs0 = 10;

int sdBtn = 2;
int sdBtnVal;

void setup() {
    Serial.begin(9600);
//    SPI.begin();
    pinMode(sdBtn, INPUT_PULLUP);
  
    delay(2000);

    Serial.print("Initializing SD card...");
  
     // see if the card is present and can be initialized:
    if (!SD.begin(cs0)) {
        Serial.println("Card failed, or not present");
       // don't do anything more:
        while (1);
    }
    Serial.println("card initialized.");

    // The line below (readSDfile) wouldn't work if SD.begin wasn't called before.
    // readSDfile can be used as conditional, it returns 'false' if the file does not exist.
//    if (cp.readSDfile("/dinnerId.csv")) {  //dinnerId.csv라는 파일 읽기
//        uint32_t *ID = (uint32_t*)cp["ID"];
//        uint32_t *tag0 = (uint32_t*)cp["tag0"];
//        uint32_t *tag1 = (uint32_t*)cp["tag1"];
//        uint32_t *tag2 = (uint32_t*)cp["tag2"];
//        uint32_t *tag3 = (uint32_t*)cp["tag3"];
//    }
//    if (ID && tag0 && tag1 && tag2) {
//        for(int row = 0; row < cp.getRowsCount(); row++) {
//            Serial.print("row = ");
//            Serial.print(row, DEC);
//            Serial.print(", ID = ");
//            Serial.print(ID[row], DEC);
//            Serial.print(", tag0 = ");
//            Serial.println(tag0[row], DEC);
//            Serial.print(", tag1 = ");
//            Serial.println(tag1[row], DEC);
//            Serial.print(", tag2 = ");
//            Serial.println(tag2[row], DEC);
//            Serial.print(", tag3 = ");
//            Serial.println(tag3[row], DEC);
//        }
//    } else {
//          Serial.println("ERROR: At least 1 of the columns was not found, something went wrong.");
//    }
//    
//    // output parsed values (allows to check that the file was parsed correctly)
//    cp.print(); // assumes that "Serial.begin()" was called before (otherwise it won't work)
//    
//    } else {
//        Serial.println("ERROR: File called '/dinnerId.csv' does not exist...");
//    }
}

void loop() {
    sdBtnVal = digitalRead(sdBtn);
    Serial.println(sdBtnVal);

    if (sdBtnVal = LOW) {
        if (cp.readSDfile("/dinnerId.csv")) {  //dinnerId.csv라는 파일 읽기
            uint32_t *ID = (uint32_t*)cp["ID"];
            uint32_t *tag0 = (uint32_t*)cp["tag0"];
            uint32_t *tag1 = (uint32_t*)cp["tag1"];
            uint32_t *tag2 = (uint32_t*)cp["tag2"];
            uint32_t *tag3 = (uint32_t*)cp["tag3"];
        }
    }
}
