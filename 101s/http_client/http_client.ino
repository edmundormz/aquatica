#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"

// WiFi parameters to be configured
const char* ssid = "Totalplay-8195";
const char* password = "81956A81qN3nEKbx";
String server_address = "http://192.168.100.135/checkStringID/";
String client_id = "81:00:74:08";
String request = server_address + client_id;

void setup(void)
{ 
  Serial.begin(9600);
  WiFi.begin(ssid, password);               // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

}
void loop() {
  if (WiFi.status() == WL_CONNECTED) {      //Check WiFi connection status
    HTTPClient http;                        //Declare an object of class HTTPClient
    http.begin(request);                    //Specify request destination
    Serial.println(request);
    int httpCode = http.GET();              //Send the request
    if (httpCode > 0) {                     //Check the returning code
      String payload = http.getString();    //Get the request response payload
      Serial.println(payload);              //Print the response payload
    }
  http.end();                               //Close connection
  }
  delay(30000);                             //Send a request every 30 seconds
  }
