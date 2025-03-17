#include <Arduino.h>

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    if (Serial.available()) {
        char command = Serial.read();
        if (command == '1') {
            digitalWrite(LED_BUILTIN, HIGH);
        } 
        else if (command == '0') {
            digitalWrite(LED_BUILTIN, LOW);
        }
    }
}
