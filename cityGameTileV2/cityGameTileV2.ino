
#define __AVR_ATtinyX4__

#include <MANCHESTER.h>
#include <SoftwareSerial.h>

SoftwareSerial TinySerial(2,3); // RX, TX

// what edges are we reading pins on 
const int READ_PIN = 7;
// edge detect
const int THRESHOLD = 800;

unsigned int counter = 0;

// what mode are we in (this is temporary)
const int WAITING = 0;
const int CONNECTING = 1;
const int CONNECTED = 2;
int mode = WAITING;

const int ID = 2;
int lastConnectedSide = -1;

const int sideConnections[6] = {0, 1, 4, 5, 6, 7};
const int connectionToSideMap[6] = {0, 1, 2, 3, 4, 5};

const long sendIDInterval = 50;
long sendIDAgain = 0;

void setup()  
{
  // Open serial communications and let us know we are connected
  TinySerial.begin(1200);
  pinMode(10, OUTPUT);
  int i = 0;
  while( i < 6) {
    pinMode(sideConnections[i], INPUT);
    i++;
  }
  
  MANCHESTER.SetTxPin(0);
  
}

void loop()
{

  if( mode == CONNECTING ) {
    
    //digitalWrite(10, HIGH);
    blink(10, 1);

    if(TinySerial.available())
    {
      int received = TinySerial.parseInt();

      TinySerial.flush();

      char c;
      unsigned char d = 5;
      itoa(received, &c, 10);

      // if this tile is the lowest, we're going to tell
      // the other tile not to bother sending
      if(ID < received) {

        TinySerial.print(ID);
        delay(100); // wait for Serial to finish up
        sendConnection(ID, received, lastConnectedSide);
        mode = CONNECTED;
        blink(10, 3);
      } 
      else if( received > ID ) 
      {
        mode = CONNECTED;
        blink(10, 2);
      }

    }
    else
    {
      
      if(millis() - sendIDAgain > sendIDInterval)
      {
        TinySerial.print(ID);
        sendIDAgain = millis();
      }
      
    }
  } 

  if(mode == CONNECTED)
  {
    // or we're are connected and we need to figure out what else is connected to us
    if(analogRead(sideConnections[lastConnectedSide]) < THRESHOLD) // we just lost our connection
    {
      lastConnectedSide = -1;
      // need to send here too
      mode = WAITING;
    }
  }

  if(mode == WAITING)
  {
    if(lastConnectedSide == -1 )
    {
      int i = 0;
      
      /*while( i < 6) {
        if(analogRead(sideConnections[i]) > THRESHOLD)
        {
          lastConnectedSide = connectionToSideMap[i];
          mode = CONNECTING;
          return;
        }
        i++;
      }*/
      
      if(analogRead(7) > THRESHOLD)
      {
        lastConnectedSide = connectionToSideMap[i];
        mode = CONNECTING;
        return;
      }
      
    }
  }
}

void blink(byte led, byte times){ // poor man's display
  for (byte i=0; i< times; i++){
    digitalWrite(led,HIGH);
    delay (250);
    digitalWrite(led,LOW);
    delay (175);
  }
}


void sendConnection( int myId, int otherId, int side ) {
  unsigned char cc[6];
  cc[0] = (unsigned char) myId; // these will be this Tiles ID
  cc[1] = ';';
  cc[2] = (unsigned char) otherId; // who touched you, will be 0 - 255
  cc[3] = ';';
  cc[4] = (unsigned char) side; // on what side

  MANCHESTER.TransmitBytes( (unsigned char) 5, &cc[0]);
}




