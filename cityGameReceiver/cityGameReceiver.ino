#include <MANCHESTER.h>

#define RxPin 2

unsigned char data;
unsigned char *datas;

void setup() {
  Serial.begin(57600);
  
  pinMode(13, OUTPUT);
  
  datas = (unsigned char*) malloc( 1 );
  
  // Set digital TX pin
  MANRX_SetRxPin(2);
  // Prepare interrupts
  MANRX_SetupReceive();
  // Begin receiving data
  MANRX_BeginReceiveBytes(1, &data);
  
  pinMode(0, OUTPUT);
  
  int c = 2;
  char d;
  itoa(c, &d, 10);
  Serial.println(d);
  
}

void loop() {
  if (MANRX_ReceiveComplete()) {
    unsigned char rcvdBytes;
    
    //unsigned char *rcvdBytes, unsigned char **data)
    MANRX_GetMessageBytes(&rcvdBytes,  &datas);
    
    MANRX_BeginReceiveBytes(1, &data);
    
    int foo = datas[0] - 48;
    
    Serial.println(foo);
  }
}


