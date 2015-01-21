#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

#define DHTPIN A0 //Pino do DHT
#define DHTTYOE DHT11 //Modelo DHT

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

IPAddress ip(192,168,1,110);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

EthernetServer server(80);
DHT dht(DHTPIN, DHTTYOE);

int Led = 2;


void setup()
{
  Ethernet.begin(mac,ip,gateway,subnet);
  server.begin();
  
  pinMode(Led, OUTPUT); 
}
void loop()
{
  EthernetClient client = server.available();
  if(client)
  {
    boolean continua = true;
    String linha = "";

    while(client.connected())
    {
      if(client.available()){
        char c = client.read();
        linha.concat(c);
  
        if(c == '\n' && continua)
        {
          client.println("HTTP/1.1 200 OK");
          
          client.println("Content-Type: application/json");
          client.println();          
         
          int iniciofrente = linha.indexOf("?");
                
          if(iniciofrente>-1){
            iniciofrente     = iniciofrente+6;
            int fimfrente    = iniciofrente+3;
            String acao    = linha.substring(iniciofrente,fimfrente);
            
 
            if      ( acao == "off"){ digitalWrite(Led, LOW); } 
            else if ( acao == "oon"){ digitalWrite(Led, HIGH);} 
            else {}
            
            float temp = dht.readTemperature();//Temperatura
            float umid = dht.readHumidity();//Umidade
            
            client.print("{");
            client.print("\"temp\":");
            client.print(temp);
            client.print(",");
            client.print("\"umid\":");
            client.print(umid);
            client.print(",");
            client.print("\"led\":");
            client.print(digitalRead(Led));
            client.println("}");
            
         }          
          break;
        }
        if(c == '\n') { continua = true; }
        else if (c != '\r') { continua = false; }
      }
    }
     delay(1);
     client.stop();
  }
}
