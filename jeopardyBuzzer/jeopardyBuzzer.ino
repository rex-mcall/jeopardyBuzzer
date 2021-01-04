
/* ---------------------------------------------------------------------------------------------- */

/*
 * Jeopardy Buzzer
 * Rex McAllister, December 2020
 * 
 * Designed to run on Esp8266 NodeMCU
 */

/* ---------------------------------------------------------------------------------------------- */

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifndef APSSID
#define APSSID "ESP8266_Access-RM"
#define APPSK "HeIsOnlyMostlyDead."
#endif

/* ---------------------------------------------------------------------------------------------- */

/* FLAGS */
bool hostArmed = true;
bool firstResponse = false;
bool armedLEDs = false;

/* Pin Numbers */
const int onboardLED = D4;
const uint8_t host = D1;
const uint8_t buzzer = D3;
const uint8_t ledPin = D8;
const uint8_t players[4] = { D2, D5, D6, D7 };

/* Extra vars */
const String playerNames[4] = { "Player One", "Player Two", "Player Three", "Player Four"};
unsigned long penaltyAssessed[sizeof(players)] = { 0, 0, 0, 0 };
int penaltyDelay = 500; //time in milliseconds to ignore the remote
int numLeds = 6;

/* ---------------------------------------------------------------------------------------------- */

/* Objects */
Adafruit_NeoPixel pixels(numLeds, ledPin, NEO_GRB + NEO_KHZ800);
uint32_t ledColors[4] = { pixels.Color(255,0,0) , pixels.Color(0,255,0) , pixels.Color(0,0,255) , pixels.Color(255,255,0) };


/* ---------------------------------------------------------------------------------------------- */

/* Prototypes */

void pollRemotes();
void resetBetweenQuestions();
void beep(int time);
void setColor(int playerNumber);

/* ---------------------------------------------------------------------------------------------- */

void setup()
{
    pinMode(onboardLED, OUTPUT);
    digitalWrite(onboardLED, HIGH); //onboard led has reverse logic

    pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, LOW);

    pinMode(host, INPUT);
    for( int i = 0; i < sizeof(players); i++) {
        pinMode(players[i], INPUT);
    }

    pixels.begin();
    pixels.setBrightness(25);
    for (int i = 0; i < numLeds; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 128, 255));
    }
    pixels.show();
    delay(100);

    Serial.begin(9600);
    Serial.println("Ready");
}

/* ---------------------------------------------------------------------------------------------- */

void loop()
{
    pollRemotes();
}

/* ---------------------------------------------------------------------------------------------- */

void pollRemotes() {
    int hostState = digitalRead(host);
    for (int i = 0; i < sizeof(players); i++) {
        int currPlayerState = digitalRead(players[i]);
        if(hostState == true) { //if host has pressed button
            if(!armedLEDs) {
                armedLEDs = true;
                digitalWrite(onboardLED, LOW);
                pixels.setPixelColor(2, pixels.Color(255,0,0));
                pixels.show();
            }
        }
        else if (hostState == false) { //host released button and resets for next round
            resetBetweenQuestions();
            digitalWrite(onboardLED, HIGH);
        }

        if(currPlayerState && hostState && !(firstResponse) && penaltyAssessed[i] == 0){ //declare first to buzz in
            firstResponse = true;
            Serial.println(playerNames[i] + " buzzed in first");
            setColor(i);
            beep(25);
            break;
        }
        else if (currPlayerState && !(hostState)) { //give penalty
            penaltyAssessed[i] = millis();
        }
        else if (penaltyAssessed[i] != 0 && (millis() - penaltyAssessed[i] > penaltyDelay)) { //remove penalty
            penaltyAssessed[i] = 0;
        }
    }
}

void resetBetweenQuestions() {
    firstResponse = false;
    for (int i = 0; i < numLeds; i++) {
        pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
    pixels.show();

    armedLEDs = false;
}

void beep(int time) {
    digitalWrite(buzzer, HIGH);
    delay(time);
    digitalWrite(buzzer, LOW);
}

void setColor(int playerNumber) {
    for (int i = 0; i < numLeds; i++) {
        pixels.setPixelColor(i, ledColors[playerNumber]);
    }
    pixels.show();
}