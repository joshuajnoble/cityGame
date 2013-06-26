
#include <MANCHESTER.h>
#include <SoftwareSerial.h>

SoftwareSerial TinySerial(2,3); // RX, TX

const int READ_PIN = 7;
unsigned int counter = 0;

const int mode = 1;

void setup()  
{
  // Open serial communications and let us know we are connected
  TinySerial.begin(1200);
  pinMode(1, OUTPUT);
  pinMode(READ_PIN, INPUT);
  MANCHESTER.SetTxPin(0);
}

void loop()
{
  
if( mode == 1 ) {

  if(TinySerial.available())
  {
    int received = TinySerial.parseInt();   //Contrary to what the Arduino playground would imply, parseIt is supported
    if(received == 3)    //So it doesn't blink 150 times when you type 150 instead of 15
    {    
      Blink(1, received);    //Blink the LED on pin 1
    }
    else
    {
      Blink(1, 1);
    }
    TinySerial.flush();
    
    char c;
    unsigned char d;
    itoa(counter, &c, 10);
    d = 1;
    
    unsigned char cc = (unsigned char) c;
    
    MANCHESTER.TransmitBytes(d, &cc);
    counter++;
  }

} else {

  if(analogRead(READ_PIN) > 10)
  {
     TinySerial.print("3");
     Blink(1, 3); 
  }
}
  
}

void Blink(byte led, byte times){ // poor man's display
  for (byte i=0; i< times; i++){
    digitalWrite(led,HIGH);
    delay (250);
    digitalWrite(led,LOW);
    delay (175);
  }
}




