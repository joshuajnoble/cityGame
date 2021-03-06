
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
const int ACK = 2;
const int CONNECTED = 3;
int mode = WAITING;

const int ID = 5;
int lastConnectedSide = -1;
int received;

boolean hasID = false;
boolean hasSendFull = false;

/////////
boolean writePinHigh = false;

const int sideConnections[6] = {0, 1, 4, 5, 6, 7};
const int connectionToSideMap[6] = {0, 1, 2, 3, 4, 5};

long sendIDInterval;
long sendIDAgain = 0;

void setup()  
{
  // Open serial communications and let us know we are connected
  TinySerial.begin(1200);
  
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  
//  int i = 0;
//  while( i < 6) {
//    pinMode(sideConnections[i], INPUT);
//    i++;
//  }
  
  MANCHESTER.SetTxPin(0);
  sendIDInterval = 100 + (ID * 10);
  
}

void loop()
{
  
  if( mode == CONNECTING ) {
    received = TinySerial.read();
    
    if(received != -1)
    {

      TinySerial.flush();            
      // if this tile is the lowest, we're going to tell
      // the other tile not to bother sending
      
      if(ID < received)
      {
        digitalWrite(9, HIGH);
        digitalWrite(10, HIGH);
        
        hasID = true;
        
        TinySerial.write(0xFF);
        delay(100); // wait for Serial to finish up
        sendConnection(ID, (unsigned char) received, lastConnectedSide);
        hasSendFull = true;
      } 
      else if(received < ID)
      {
        
        hasID = true;
        
        digitalWrite(9, HIGH);
        TinySerial.write(0xFF);
        hasSendFull = true;
      }
      else if( received == 0xFF && hasSendFull)
      {
        mode = CONNECTED;
      }
      else if( received == 0xFF && !hasSendFull)
      {
        TinySerial.write(0xFF);
        mode = CONNECTED;
      }
    }
    else
    {
      if(millis() - sendIDAgain > sendIDInterval)
      {
        TinySerial.write(ID);
        sendIDAgain = millis();
        TinySerial.flush();
      }
    }
  }

  if(mode == CONNECTED)
  {
    
    //Blink(10, received);
    digitalWrite(9, HIGH);
    /*
    // or we're are connected and we need to figure out what else is connected to us
    if(analogRead(sideConnections[lastConnectedSide]) < THRESHOLD) // we just lost our connection
    {
      lastConnectedSide = -1;
      // need to send here too
      mode = WAITING;
    }*/
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
        lastConnectedSide = 3;
        mode = CONNECTING;
        return;
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


void sendDisconnect( int myId, int otherId, int side ) {
  unsigned char cc[7];
  cc[0] = (unsigned char) myId; // these will be this Tiles ID
  cc[1] = ';';
  cc[2] = (unsigned char) otherId; // who touched you, will be 0 - 255
  cc[3] = ';';
  cc[4] = (unsigned char) side; // on what side

  MANCHESTER.TransmitBytes( (unsigned char) 5, &cc[0]);
}

void sendConnection( int myId, int otherId, int side ) {
  unsigned char cc[7];
  cc[0] = (unsigned char) myId; // these will be this Tiles ID
  cc[1] = ';';
  cc[2] = (unsigned char) otherId; // who touched you, will be 0 - 255
  cc[3] = ';';
  cc[4] = (unsigned char) side; // on what side

  MANCHESTER.TransmitBytes( (unsigned char) 5, &cc[0]);
}


void signal(int pin, int times) {
 for( int i = 0; i < times; i++) {
   digitalWrite(pin, HIGH);
   digitalWrite(pin, LOW);
 } 
}


