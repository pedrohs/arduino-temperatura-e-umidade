#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

#define DHTPIN A0 //Pino do DHT
#define DHTTYOE DHT11 //Modelo DHT

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 110 };
     
EthernetServer server(80);
DHT dht(DHTPIN, DHTTYOE);

String readString;
int Led = 2;
  
void setup(){
  dht.begin();
  pinMode(Pin, OUTPUT);
  Ethernet.begin(mac, ip);
  server.begin();
}
  
void loop(){
  float temp = dht.readTemperature();//Temperatura
 float umid = dht.readHumidity();//Umidade

  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
  
        if (readString.length() < 100) {
          readString += c;              
        }
 
        if (c == '\n') {
         client.println("HTTP/1.1 200 OK");
         client.println("Content-Type: application/json");
         client.println();
         client.print("{");
         client.print("\"temp\":");
         client.print(temp);
         client.print(",");
         client.print("\"umid\":");
         client.print(umid);
         client.println("}");
           
          delay(1);
          client.stop();
           
          if(readString.indexOf("?ledon") > 0)
          {
            digitalWrite(Led, HIGH);
          }
          else {
            if(readString.indexOf("?ledoff") > 0)
            {
              digitalWrite(Led, LOW);
            }
          }
          readString="";     
        }
      }
    }
  }
}
