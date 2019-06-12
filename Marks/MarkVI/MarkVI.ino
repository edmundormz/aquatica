#include <ESP8266WiFi.h>                  //Basic ESP8266 library
#include <ESP8266WiFiMulti.h>             //To connect to different networks
#include <ESP8266HTTPClient.h>            //For making HTTP requests
#include <ESP8266WebServer.h>             //To enable web service and upload scripts
#include <ESP8266mDNS.h>                  //To enable multicast DNS service
#include <ESP8266HTTPUpdateServer.h>      //To manage script updating via web
#include <SPI.h>                          //Required to communicate with NFC reader
#include <MFRC522.h>                      //To manage NFC reader

#define SS_PIN 4                            //D2 (NFC reader required)
#define RST_PIN 5                           //D1 (NFC reader required)
#define relay1 15                           //D8
#define relay2 2                            //D4
#define led_404 0                           //D3
#define wifi_led 16                         //D0 BUILTINLED
#define access_relay_timing 1000                   //Time to leave the relays closed
#define block_relay_timing 1000

ESP8266WiFiMulti wifiMulti;                 //WiFi Multi object
MFRC522 mfrc522(SS_PIN, RST_PIN);           //MFRC522 object
HTTPClient http;                            //HTTPClient object
ESP8266WebServer httpServer(80);            //Opens web server on port 80
ESP8266HTTPUpdateServer httpUpdater;        //UpdateServer object

//MRFC522 Variables
String content;
byte letter;

//Network variables
String node_name = "node_" + String(ESP.getChipId());
String server_addresses[2] = {"http://192.168.0.150/check_user_id/", "http://142.93.93.25/api/Students/canPass?nfcId="};
String client_id = "";
String request = "";

void setup(void)
{ 
  //System initialization
  //Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(led_404, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(wifi_led, OUTPUT);
  //http.setTimeout(1000);

  //Turn off outputs
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(wifi_led, HIGH);
  digitalWrite(led_404, HIGH);
}


void connectWiFi(){
  WiFi.mode(WIFI_AP_STA);
  wifiMulti.addAP("TorniquetesDelfines", "Aquatica");
  //wifiMulti.addAP("AcuaticaDelfines", "CorchoTiburon");
  wifiMulti.addAP("HecMundo", "password");
  while (wifiMulti.run() != WL_CONNECTED) {
    digitalWrite(wifi_led, LOW);
    delay(100);
    digitalWrite(wifi_led, HIGH);
    delay(100);
  }
  //WiFi.begin(ssid, password);   
  WiFi.hostname(node_name);
  //Serial.println(node_name);
  MDNS.begin(node_name);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  return;
}

void processRFID(){
  for(byte i = 0; i < mfrc522.uid.size; i++){
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  client_id = content;
  content = "";
  return;
}

void blink_led(bool access){
  if (access == true){
        digitalWrite(relay2, HIGH);
        digitalWrite(wifi_led, LOW);
        delay(access_relay_timing);
        digitalWrite(relay2, LOW);
        digitalWrite(wifi_led, HIGH);
      }
      else{
        digitalWrite(relay1, HIGH);
        digitalWrite(led_404, LOW);
        delay(block_relay_timing);
        digitalWrite(relay1, LOW);
        digitalWrite(led_404, HIGH);
      }
  return;
}

void webRequest(){
    for (int i=0; i<2; i++){
      request =  server_addresses[i] + client_id;
      //Serial.println(request);
      http.begin(request);                    //Specify request destination
      int httpCode = http.GET();              //Send the request
      if (httpCode > 0) {                     //Check the returning code
        String payload = http.getString();    //Get the request response payload
        //Serial.println(payload);
        if (payload == "true"){
          http.end();
          blink_led(true);  //Access granted
        }
        else{
          http.end();
          blink_led(false); //Access denied
        }
        break;
      }
      else{
        http.end();
        //Serial.println("ServerNotFound");
        for(int i = 0; i<3; i++){
          digitalWrite(led_404, LOW);
          delay(200);
          digitalWrite(led_404, HIGH);
          delay(200);
          }
      }
    //http.end();                               //Close connection
    }
  return;
}

void loop(){
  httpServer.handleClient();
  MDNS.update();
  //Check WiFi connection
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }
  
  //RFID Card detection
  if(!mfrc522.PICC_IsNewCardPresent()){    //Look for cards
    return;
  }
  if(!mfrc522.PICC_ReadCardSerial()){       //Select one of the cards
    return;
  }
  
  processRFID();
  webRequest();
  }
