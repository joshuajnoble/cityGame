#include <MANCHESTER.h>

unsigned char *datas;

void setup() {
  Serial.begin(57600);
  
  datas = (unsigned char*) malloc( 4 );

  MANRX_SetRxPin(2);

  // Prepare interrupts
  MANRX_SetupReceive();

  // Begin receiving data
  MANRX_BeginReceiveBytes(4, datas);
  pinMode(13, OUTPUT);
  //Serial.println(" OK ");
}

void loop() {
  

  if (MANRX_ReceiveComplete()) {
    
    digitalWrite(13, HIGH);
    unsigned char rcvdBytes;

    //MANRX_GetMessageBytes(&rcvdBytes,  &datas);
    
    Serial.print(" rcvd: ");
    Serial.println(rcvdBytes);
    
    int firstTile = datas[1];
    int secondTile = datas[2];
    int side = datas[3];
    
    Serial.print(firstTile);
    Serial.print(" ");
    Serial.print(secondTile);
    Serial.print(" ");
    Serial.println(side);
    
    MANRX_BeginReceiveBytes(4, datas);
  }
  
}


