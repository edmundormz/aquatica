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
#define relay_timing 2000                   //Time to leave the relays closed


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
String server_addresses[2] = {"http://192.168.100.106/check_user_id/", "http://142.93.93.25/api/Students/canPass?nfcId="};
String client_id = "";
String request = "";

void setup(void)
{ 
  //System initialization
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(led_404, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(wifi_led, OUTPUT);
  http.setTimeout(1000);

  //Turn off outputs
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(wifi_led, HIGH);
  digitalWrite(led_404, HIGH);
}


void connectWiFi(){
  WiFi.mode(WIFI_STA);                      //Defines ESP8266 as a client, not station
  wifiMulti.addAP("Totalplay-8195", "81956A81qN3nEKbx");
  //wifiMulti.addAP("PidemeLaContrasenia", "LadyChewbaca1001");
  while (wifiMulti.run() != WL_CONNECTED) {
    digitalWrite(wifi_led, LOW);
    delay(250);
    digitalWrite(wifi_led, HIGH);
    delay(250);
  }
  WiFi.hostname(node_name);
  //Serial.println("");
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
  //Testing IDs: 22f86c0e, 81007408, 199059D3 //62309331  //199059D3
  //content.toUpperCase();
  client_id = content;
  content = "";
  return;
}

void blink_led(bool access){
  if (access == true){
        digitalWrite(relay2, HIGH);
        //digitalWrite(relay1, HIGH);
        delay(relay_timing);
        digitalWrite(relay2, LOW);
        //digitalWrite(relay1, LOW);
      }
      else{
        digitalWrite(led_404, LOW);
        delay(2000);
        digitalWrite(led_404, HIGH);
      }
  return;
}

void webRequest(){
    for (int i=0; i<2; i++){
    request =  server_addresses[i] + client_id;
    Serial.println(request);
    http.begin(request);                    //Specify request destination
    int httpCode = http.GET();              //Send the request
    if (httpCode > 0) {                     //Check the returning code
      String payload = http.getString();    //Get the request response payload
      Serial.println(payload);
      if (payload == "true"){
        blink_led(true);  //Access granted
      }
      else{
        blink_led(false); //Access denied
      }
      break;
    }
    //second server address
    else{
      Serial.println("ServerNotFound");
      for(int i = 0; i<5; i++){
        digitalWrite(led_404, LOW);
        delay(200);
        digitalWrite(led_404, HIGH);
        delay(200);
        }
    }
  http.end();                               //Close connection
    }
  return;
}

void loop(){
  
  //Check WiFi connection
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }

  httpServer.handleClient();
  MDNS.update();
  
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
