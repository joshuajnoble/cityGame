#include <SoftEasyTransfer.h>

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

const int ID = 3;
int lastConnectedSide = -1;
int received;

boolean hasID = false;
boolean hasSentData = false;

SoftEasyTransfer ET;

struct connection{
  int ID;
  int side;
};

connection conn;

/////////
boolean writePinHigh = false;

const int sideConnections[6] = {0, 1, 4, 5, 6, 7};
const int connectionToSideMap[6] = {0, 1, 2, 3, 4, 5};

long sendIDInterval;
long sendIDAgain = 0;

void setup()  
{
  // Open serial communications and let us know we are connected
  TinySerial.begin(4800);
  
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  
//  int i = 0;
//  while( i < 6) {
//    pinMode(sideConnections[i], INPUT);
//    i++;
//  }
  
  MANCHESTER.SetTxPin(0);
  sendIDInterval = 100 + (ID * 10);
  
  
  ET.begin(details(conn), &TinySerial);
  
}

void loop()
{
  
  if( mode == CONNECTING ) 
  {
    
    //signal(10, 1);
    
    // this is a blocking call
    if( ET.receiveData() )
    {
      
      signal(10, conn.ID);
      
      if(ID < conn.ID)
      {
        hasID = true;
        delay(200); // wait for Serial to finish up
        sendConnection(ID, (unsigned char) conn.ID, lastConnectedSide);
        digitalWrite(10, HIGH);
      } 
      else if(conn.ID < ID)
      {
        hasID = true;
      }
      return; // quit
    }
    
    if( !hasSentData )
    {
      
      conn.ID = ID;
      conn.side = 4;
      
      ET.sendData();
      
      //signal(10, 2);
      
      hasSentData = true;
    }
    
    if( hasID && hasSentData )
    {
      mode = CONNECTED;
    }
    
  }

  if(mode == CONNECTED)
  {
    digitalWrite(9, HIGH);
    /*
    // or we're are connected and we need to figure out what else is connected to us
    if(analogRead(sideConnections[lastConnectedSide]) < THRESHOLD) // we just lost our connection
    {
      lastConnectedSide = -1;
      // need to send here too
      mode = WAITING;
      hasID = false;
      hasSentData = false;
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
  cc[0] = '-';
  cc[1] = (unsigned char) myId; // these will be this Tiles ID
  cc[2] = ';';
  cc[3] = (unsigned char) otherId; // who touched you, will be 0 - 255
  cc[4] = ';';
  cc[5] = (unsigned char) side; // on what side

  MANCHESTER.TransmitBytes( (unsigned char) 6, &cc[0]);
}

void sendConnection( int myId, int otherId, int side ) {
  
  unsigned char cc[7];
  cc[0] = '+';
  cc[1] = (unsigned char) myId; // these will be this Tiles ID
  cc[2] = ';';
  cc[3] = (unsigned char) otherId; // who touched you, will be 0 - 255
  cc[4] = ';';
  cc[5] = (unsigned char) side; // on what side

  MANCHESTER.TransmitBytes( (unsigned char) 6, &cc[0]);
}


void signal(int pin, int times) {
 for( int i = 0; i < times; i++) {
   digitalWrite(pin, HIGH);
   digitalWrite(pin, LOW);
 } 
}


