#include <WiFi.h>
#include "ESPAsyncWebServer.h"

const char* ssid     = "ESP32";
const char* password = "esp-password";

AsyncWebServer server(80);

//initializing and declaring led rows --> pins for collumns and layers
  int column[16]={23,18,32,16,22,5,33,17,21,14,25,15,19,27,26,1};
//initializing and declaring led layers
  int layer[4]={13,12,4,2};

  int matrix[][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
  };
 
void setup()
{
  delay(1000);
  
  //setting rows to ouput
  for(int i = 0; i<16; i++)
  {
    pinMode(column[i], OUTPUT);
  }
  //setting layers to output
  for(int i = 0; i<4; i++)
  {
    pinMode(layer[i], OUTPUT);
  }

  //Wifi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
 
    int paramsNr = request->params();
 
    for(int i=0;i<paramsNr;i++){
 
        AsyncWebParameter* p = request->getParam(i);
        getMatrix(p->value());
    }
 
    request->send(200, "text/plain", "ok");
  });
  server.begin();
}

//xxxxxxxxxxxxxxxxxxxxFUNCTION LOOPxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void loop()
{ 
  turnOnMatrix(matrix, 10);
}

//xxxxxxxxxxxxxxxxxxxxFUNCTIONSxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

///////////////////////////////////////////////////////////turn all off
void turnEverythingOff()
 {
   for(int i = 0; i<16; i++)
   {
     digitalWrite(column[i], 1);
   }
   for(int i = 0; i<4; i++)
   {
     digitalWrite(layer[i], 0);
   }
 }
 
////////////////////////////////////////////////////////////turn all on
void turnEverythingOn()
{
  for(int i = 0; i<16; i++)
  {
    digitalWrite(column[i], 0);
  }
  //turning on layers
  for(int i = 0; i<4; i++)
  {
    digitalWrite(layer[i], 1);
  }
}

//////////////////////////////////////////////////////////////////////Get matrix from string input

void getMatrix(String param){
  for(int i = 0; i < 4; i++){
    for(int k = 0; k < 16; k++){
      matrix[i][k] = String(param[i*16 + k]).toInt();
    }
  }
}

//////////////////////////////////////////////////////////////////////turn on matrix
//[15,11,07,03]-> colmumn layout
//[14,10,06,02]
//[13,09,05,01]
//[12,08,04,00]
void turnOnMatrix(int matrix[][16], int mlsc) /// mlsc == 1/4 of display time
{
  for(int i = 0; i < mlsc; i++){
    for(int lay = 0; lay < 4; lay++){
      turnEverythingOff();
      digitalWrite(layer[lay], 1);
      for(int col = 0; col < 16; col++){
        if(matrix[lay][col]==1){
          digitalWrite(column[col], 0);
        } 
      }
      delay(2);    
    }
  }
}

