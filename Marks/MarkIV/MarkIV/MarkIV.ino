#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 4                            //D2
#define RST_PIN 5                           //D1
#define relay1 15                           //D8
#define relay2 2                            //D4
#define led_404 0                           //D3
#define wifi_led 16                         //D0 BUILTINLED
#define relay_timing 2000


MFRC522 mfrc522(SS_PIN, RST_PIN);           //MFRC522 object
HTTPClient http;                            //HTTPClient object

//MRFC522 Variables
String content;
byte letter;

//WiFi variables
//const char* ssid = "Totalplay-8195";
//const char* password = "81956A81qN3nEKbx";
//const char* ssid = "IOT-Lab";
//const char* password = "InnovationLab-987";
const char* ssid = "PidemeLaContrasenia";
const char* password = "LadyChewbaca1001";
//String server_address = "http://192.168.0.104/checkStringID/";
String server_address = "http://142.93.93.25/api/Students/canPass?nfcId=";
String client_id = "";
String request = "";


void setup(void)
{ 
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(led_404, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(wifi_led, OUTPUT);
}

void connectWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);               // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(wifi_led, LOW);
    delay(250);
    digitalWrite(wifi_led, HIGH);
    delay(250);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  
  return;
}

void processRFID(){
  for(byte i = 0; i < mfrc522.uid.size; i++){
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Testing IDs: 22f86c0e, 81007408, 199059D3 //62309331  //199059D3
  content.toUpperCase();
  Serial.print(content + ",");
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
    request =  server_address + client_id;
    //Serial.println(request);
    http.begin(request);                    //Specify request destination
    int httpCode = http.GET();              //Send the request
    if (httpCode > 0) {                     //Check the returning code
      String payload = http.getString();    //Get the request response payload
      Serial.println(payload);              //Print the response payload
      if (payload == "true"){
        blink_led(true);  //Access granted
      }
      else{
        blink_led(false); //Access denied
      }
    }
    else{
      Serial.println("ServerNotFound");
      for(int i = 0; i<5; i++){
        digitalWrite(led_404, LOW);
        delay(100);
        digitalWrite(led_404, HIGH);
        delay(100);
        }
    }
  http.end();                               //Close connection
  return;
}

void loop() {
  //Turn off outputs
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(wifi_led, HIGH);
  digitalWrite(led_404, HIGH);
  
  
  //Check WiFi connection
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }
  
  //RFID Card detection
  if (!mfrc522.PICC_IsNewCardPresent()){    //Look for cards
    return;
  }
  if(!mfrc522.PICC_ReadCardSerial()){       //Select one of the cards
    return;
  }
  processRFID();
  webRequest();
  //Serial.println("End of process");
  //delay(2000);
  }
