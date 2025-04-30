#include <Arduino.h>
#include <Servo.h>

Servo swingArm;
bool isRunning = false;
unsigned long lastUpdate = 0;
unsigned long stepInterval = 15;
int speed = 60;
int direction = 1;
int sweepMin = 40;
int sweepMax = 150;

bool isPaused = false;
unsigned long pauseStart = 0;
unsigned long pauseDuration = 0;

void setup() {
    swingArm.attach(9);
    Serial.begin(9600);
    Serial.println("> Send '1' to START swing arm, '0' to STOP.");
}

void loop() {
    // Serial command input
    if (Serial.available()) {
        char cmd = Serial.read();
        if (cmd == '1') {
            isRunning = true;
            Serial.println("> SWING ARM STARTED");
        } else if (cmd == '0') {
            isRunning = false;
            isPaused = false;
            swingArm.write(90);
            Serial.println("> SWING ARM STOPPED");
        }
    }

    if (isRunning) {
        // Check if we're paused
        if (isPaused) {
            if (millis() - pauseStart >= pauseDuration) {
                isPaused = false;
                Serial.println("> Resuming swing...");
            } else {
                return; // stay paused
            }
        }

        // Time to update speed
        if (millis() - lastUpdate >= stepInterval) {
            lastUpdate = millis();
            speed += direction;

            // If limit reached, reverse direction
            if (speed >= sweepMax || speed <= sweepMin) {
                direction *= -1;

                // Random chance to pause
                if (random(0, 5) == 0) { // ~20% chance
                    isPaused = true;
                    pauseDuration = random(1500, 3000); // 1.5 to 2 seconds
                    pauseStart = millis();
                    Serial.println("> Random pause...");
                    swingArm.write(90); // stop
                    return;
                }
            }

            swingArm.write(speed);
        }
    }

    if (!isRunning) {
        swingArm.write(90); // Always stop when inactive
    }
}

