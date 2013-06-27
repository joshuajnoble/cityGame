
#include <MANCHESTER.h>
#include <SoftwareSerial.h>

SoftwareSerial TinySerial(2,3); // RX, TX

// what edges are we reading pins on 
const int READ_PIN = 7;
// edge detect
const int THRESHOLD = 15;

unsigned int counter = 0;

// what mode are we in (this is temporary)
const int WAITING = 0;
const int CONNECTING = 1;
const int CONNECTED = 2;
int mode = WAITING;

const int ID = '3';
int lastConnectedSide = -1;

const int sideConnections[6] = {0, 1, 4, 5, 6, 7};
const int connectionToSideMap[6] = {0, 1, 2, 3, 4, 5};

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

  if( mode == CONNECTING ) {

    if(TinySerial.available())
    {
      int received = TinySerial.parseInt();
      if(received > 0 && received < 64) // how many tiles will we have?
      {    
        Blink(1, received); //debug confirmation
      }

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
      } 
      else if( received > ID ) 
      {
        mode = CONNECTED;
      }

    }
    else
    {
      TinySerial.print(ID);
      mode = CONNECTING;
    }
  } 

  if(mode == CONNECTED)
  {
    // or we're are connected and we need to figure out what else is connected to us
    if(analogRead(sideConnections[lastConnectedSide]) < THRESHOLD) // we just lost our connection
    {
      lastConnectedSide = -1;
      mode = WAITING;
    }
  }

  if(mode == WAITING)
  {
    // two cases: we're not connected yet:
    if(lastConnectedSide == -1 )
    {
      int i = 0;
      while( i < 6) {
        if(analogRead(sideConnections[i]) > THRESHOLD)
        {
          lastConnectedSide = connectionToSideMap[i];
          mode = CONNECTING;
        }
        i++;
      }
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


void sendConnection( int myId, int otherId, int side ) {
  unsigned char cc[6];
  cc[0] = (unsigned char) myId; // these will be this Tiles ID
  cc[1] = ';';
  cc[2] = (unsigned char) otherId; // who touched you, will be 0 - 255
  cc[3] = ';';
  cc[4] = (unsigned char) side; // on what side

  MANCHESTER.TransmitBytes( (unsigned char) 5, &cc[0]);
}




