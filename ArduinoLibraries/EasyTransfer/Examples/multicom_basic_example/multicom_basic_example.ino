#include <EasyTransfer.h>

// you can use softserial like this
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(2,3);


// MULTICOM VARIABLES
char currentNode = 0;
int timeout = 0;
bool moveToNextSlave = false;

#define NODEID 0 // the id of current arduino board
#define NUMNODES 3 // inclusive master node 

//  PARAMS for multicom
#define PLEASETALK 0
#define PANIC 1
#define DONETALKING 2
#define MASTERNODEID 0




//create object
EasyTransfer ET; 

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  char from;
  char to;
  int value;
  int param;
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

void setup(){
  Serial1.begin(9600);
  Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
  ET.begin(details(mydata), &Serial1);

  pinMode(13, OUTPUT);
  Serial.println("starting");

}

int testValue = 0;

void loop(){
  


  multicom_update();

}



void multicom_talk()
{
 //multicom_send(MASTERNODEID,5,testValue++);
}

void multicom_receive()
{
  if(mydata.param == PANIC)
  {
    // PANIC FROM NODE mydata.from 
  }
  if(mydata.param == 5)
  {
    Serial.print((byte)mydata.from);  
    Serial.print("  ");    
    Serial.println(mydata.value); 
  }
  
}

void multicom_send(char to, int param,int value)
{
  mydata.from = NODEID;
  mydata.to = to;
  mydata.param = param;
  mydata.value = value;
  ET.sendData(); 
}

void debug()
{
 
     Serial.print((byte)mydata.from);  
     Serial.print("  ");    
      Serial.print((byte)mydata.to);  
    Serial.print("  "); 
    Serial.print(mydata.param);  
    Serial.print("  ");    
   
    Serial.println(mydata.value);  
}

void multicom_update()
{
 
  if(NODEID == MASTERNODEID)
  {
     
   
    timeout = timeout -1;
    if(timeout < 0 || moveToNextSlave)
    {
      if(timeout <0)
      {
       Serial.println("timeout"); 
      }
      currentNode =(currentNode + 1) % (NUMNODES);
      timeout = 3000;
      moveToNextSlave = false;
      multicom_send(currentNode,PLEASETALK,0);
     
     
    }

  }
 //Serial.println("sdf");
 
  while(ET.receiveData())
  {
    
  //debug();
    if(mydata.to == NODEID) //we got something.
    {
       
      if(mydata.param == PLEASETALK)
      {
     
        //We can talk
        multicom_talk();
        //Let master know that we are done   
        multicom_send(MASTERNODEID,DONETALKING,0); 
      }
      else if(NODEID == MASTERNODEID && mydata.param == DONETALKING) // we are master
      {
        moveToNextSlave = true;
         
      }
      else
      {
          
        multicom_receive();
      }


    } 
  }
 

}












