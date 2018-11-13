#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 4                            //D2
#define RST_PIN 5                           //D1
#define access_led 9 //GPIO_09
#define block_led 10 //GPIO_10
#define relay 16

MFRC522 mfrc522(SS_PIN, RST_PIN);           //MFRC522 object
HTTPClient http;                            //HTTPClient object

//MRFC522 Variables
String content;
byte letter;

//WiFi variables
const char* ssid = "Totalplay-8195";
const char* password = "81956A81qN3nEKbx";
//const char* ssid = "PidemeLaContrasenia";
//const char* password = "LadyChewbaca1001";
String server_address = "http://192.168.100.135/checkStringID/";
String client_id = "";
String request = "";


void setup(void)
{ 
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  //pinMode(access_led, OUTPUT);
  //pinMode(block_led, OUTPUT);
}

void connectWiFi(){
  WiFi.begin(ssid, password);               // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void processRFID(){
  for(byte i = 0; i < mfrc522.uid.size; i++){
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println(content);
  client_id = content;
  content = "";
  return;
}

void blink_led(bool access){
  if (access == true){
        digitalWrite(access_led, HIGH);
        delay(500);
        digitalWrite(access_led, LOW);
      }
      else{
        for(int i = 0; i<5; i++){
        digitalWrite(block_led, HIGH);
        delay(100);
        digitalWrite(block_led, LOW);
        delay(100);
        }
      }
  return;
}

void webRequest(){
    request =  server_address + client_id;
    Serial.println(request);
    http.begin(request);                    //Specify request destination
    int httpCode = http.GET();              //Send the request
    //http.end();
    if (httpCode > 0) {                     //Check the returning code
      String payload = http.getString();    //Get the request response payload
      Serial.println(payload);              //Print the response payload
      if (payload == "Yes"){
        blink_led(true);
      }
      else{
        blink_led(false);
      }
    }
  http.end();                               //Close connection
  return;
}

void loop() {
  //Check WiFi connection
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
    return;
  }

  //Turn off LEDs
  //digitalWrite(access_led, LOW);
  //digitalWrite(block_led, LOW);
  
  //RFID Card detection
  if (!mfrc522.PICC_IsNewCardPresent()){    //Look for cards
    return;
  }
  if(!mfrc522.PICC_ReadCardSerial()){       //Select one of the cards
    return;
  }
  processRFID();
  webRequest();
  Serial.println("X");
  //delay(2000);
  }
