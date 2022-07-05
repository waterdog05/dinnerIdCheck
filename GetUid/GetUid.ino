#include <SPI.h>
#include <MFRC522.h>

const int rstPin = 8;
const int cs1 = 9;

MFRC522 mfrc(cs1, rstPin);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    SPI.begin();

    mfrc.PCD_Init();
}

void loop() {
    // put your main code here, to run repeatedly:
    if (!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial()) {

        delay(500);
        return 0;
    }

    Serial.print("Card UID : ");

    for (byte i=0; i<4; i++) {
        Serial.print(mfrc.uid.uidByte[i]);
        Serial.print(" ");
    }
    Serial.println();
}
