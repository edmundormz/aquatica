int incomingByte = 0;   // for incoming serial data

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
}

void loop() {

        // send data only when you receive data:
        if (Serial.available() > 0) {
              // read the incoming byte:
              incomingByte = Serial.read();
              incomingByte = incomingByte - 48;
              Serial.print("I received: ");
              Serial.println(incomingByte);
        }
        if (incomingByte == 0){
          digitalWrite(LED_BUILTIN, HIGH);  //Outputs are inverted
        }
        else if (incomingByte == 1){
          digitalWrite(LED_BUILTIN, LOW); //Outputs are inverted
        }
}
