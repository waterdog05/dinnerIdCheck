/*
    dinnerId.csv 양식
          
    ID,tag0,tag1,tag2,tag3\n
    1101,245,123,256,764\n
    1102,224,164,194,299\n
    1103,333,558,192,168\n
*/
#include <CSV_Parser.h>
#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>

const int cs0 = 10;  //cs for sd

const int cs1 = 9;  //cs for rfid
const int rstPin = 8;

int dinRegister;  //1-석식 신청, 0-신청 안함

CSV_Parser cp(/*format*/ "LLLLL", /*has_header*/ true, /*delimiter*/ ',');
MFRC522 mfrc(cs1, rstPin);

void Ydin();  //신청 o
void Ndin();  //신청 x
void reset();  //초기화

int32_t *ID ;  //학번
int32_t *tag0;  //111
int32_t *tag1;  //222
int32_t *tag2;  //333
int32_t *tag3;  //444

void setup() {
    Serial.begin(9600);
    SPI.begin();
    
    mfrc.PCD_Init();
    delay(1000);

    Serial.println(F("Initializing SD card..."));  //나중에 lcd로 교체
  
     // see if the card is present and can be initialized:
    if (!SD.begin(cs0)) {
        Serial.println(F("Card failed, or not present"));
       // don't do anything more:
        while (1);
    }

    Serial.println(F("reading sd card"));
    delay(1000);
    
    if (cp.readSDfile("/dinnerId.csv")) {  //dinnerId.csv라는 파일 읽기
        Serial.println(F("/dinnerId.csv found"));
        Serial.println(F("analyzing /dinnerId.csv"));
        
        //dinner.csv 양식 -> 학번,111,222,333,444
        ID =(int32_t*)cp["ID"];  //학번
        tag0 = (int32_t*)cp["tag0"];  //111
        tag1 = (int32_t*)cp["tag1"];  //222
        tag2 = (int32_t*)cp["tag2"];  //333
        tag3 = (int32_t*)cp["tag3"];  //444

        if (ID && tag0 && tag1 && tag2 && tag3) {  //csv parser test
//            for(int row = 0; row < cp.getRowsCount(); row++) {
//                Serial.print("row = ");
//                Serial.print(row, DEC);
//                
//                Serial.print(", ID = ");
//                Serial.print(ID[row], DEC);
//                
//                Serial.print(", tag0 = ");
//                Serial.print(tag0[row], DEC);
//                
//                Serial.print(", tag1 = ");
//                Serial.print(tag1[row], DEC);
//                
//                Serial.print(", tag2 = ");
//                Serial.print(tag2[row], DEC);
//                
//                Serial.print(", tag3 = ");
//                Serial.println(tag3[row], DEC);
//                
//                Serial.println();
//            }
            Serial.println("Please wait");
            Serial.println("ready to use");
            delay(1000);
        } else {
            Serial.println("ERROR: Some columns not found, something went wrong.");
        }
        
        // output parsed values (allows to check that the file was parsed correctly)
//        cp.print(); // assumes that "Serial.begin()" was called before (otherwise it won't work)
    } else {
        Serial.println("ERROR: File called '/dinnerId.csv' does not exist...");
    }
    delay(1000);
    
}

void loop() {
    // The line below (readSDfile) wouldn't work if SD.begin wasn't called before.
    // readSDfile can be used as conditional, it returns 'false' if the file does not exist.

//    if (!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial()) {
//        return 0;
//    }

    Serial.print("card Uid : ");

    for (byte i=0; i<4; i++) {
        Serial.print(mfrc.uid.uidByte[i]);  //rfid test
        Serial.print(" ");
    }
    Serial.println();

    for(int row = 0; row < cp.getRowsCount(); row++) {
        if (((mfrc.uid.uidByte[0]==tag0[row]) && (mfrc.uid.uidByte[1]==tag1[row])) && ((mfrc.uid.uidByte[2]==tag2[row]) && (mfrc.uid.uidByte[3]==tag3[row]))) {
            dinRegister = 1;
        } else {
            dinRegister = 0;
        }
    }
    Serial.println(dinRegister);
    Serial.println();

    if (dinRegister == 1) {
        Ydin();
    } else if (dinRegister == 0) {
        Ndin();
    } else {
        return;
    }

    reset();
}

void Ydin() {
    /*
     초록색 led, lcd에 학번과 신청했다고 표시, 부저
    */
    Serial.println("yes");
    Serial.println();
}

void Ndin() {
    /*
     빨간색 led, lcd에 신청 안 했다고 표시, 부저
    */
    Serial.println("no");
    Serial.println();
}

void reset() {
    dinRegister = 3;
}
