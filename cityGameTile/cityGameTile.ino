
#include <MANCHESTER.h>
#include <SoftwareSerial.h>

SoftwareSerial TinySerial(2,3); // RX, TX

// what edges are we reading pins on 
const int READ_PIN = 7;
// edge detect
const int THRESHOLD = 15;

unsigned int counter = 0;

// what mode are we in (this is temporary)
const int READ = 1;
const int WRITE = 2;
const int mode = READ;

const int ID = '3';

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

  if( mode == READ ) {

    if(TinySerial.available())
    {
      int received = TinySerial.parseInt();
      if(received > 0 && received < 24) // how many tiles will we have?
      {    
        Blink(1, received); //debug confirmation
      }

      TinySerial.flush();

      char c;
      unsigned char d = 5;
      itoa(received, &c, 10);

      unsigned char cc[8];
      cc[0] = (unsigned char) ID; // these will be this Tiles ID
      cc[1] = ';';
      cc[2] = (unsigned char) c; // who touched you, will be 0 - 255
      cc[3] = ';';
      cc[4] = '3'; // on what side
      

      MANCHESTER.TransmitBytes(d, &cc);
      counter++;
    }

  } 
  else {

    if(analogRead(READ_PIN) > THRESHOLD)
    {
      TinySerial.print(ID);
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





