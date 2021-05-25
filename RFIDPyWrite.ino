
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

byte sector         = 0;
byte blockAddr      = 0;
byte trailerBlock   = 7;

byte dataBlockFName[48];
byte dataBlockLName[48];
byte dataBlockIDNumber[48];
byte dataBlockGradeLevel[16];
byte dataBlockAccessLevel[16];

String sepString[16];


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  // Prepare the key (used both as key A and as key B)
  // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
  for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
  }


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Loop");
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    if(checkForNewData()){
        delimiterData(Serial.readString());
    }
  
  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);

  //Auth
  status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
  }
  
  // Read data from the block (again, should now be what we have written)
  //Serial.print(F("Reading data from block ")); 
  //Serial.print(blockAddr);
  //Serial.println(F(" ..."));
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
  }
  mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  //Serial.print(F("Data in block "));
  //Serial.print(blockAddr);
  //Serial.println(F(":"));
  for(byte i = 0; i < 2;i++){
    
    dump_byte_array(buffer, 16); Serial.println();
  }
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void delimiterData(String data){
  //String strSepData = "";
  byte startIndex = 0;
  byte endIndex = 0;
  byte numData = 0;
  for(byte i = 0; i < data.length(); i+=6){
    if(data.substring(i,i+6) == "0x02, "){
      startIndex = i;
      
    }
    if(data.substring(i,i+6) == "0x03"){
      endIndex = i;
      sepString[numData] = data.substring(startIndex, endIndex+6);
      numData ++;
    }
    
  }
}
//checks for new data, returns true if there is new dada
bool checkForNewData(){
  if(Serial.available() > 0){
    delay(1000);
    return true;
  }
}
