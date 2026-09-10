#include <Adafruit_NeoPixel.h>

#include "color.h"
#include "image.h"

#define SENSOR_PIN 3
#define LED_PIN 4

#define LED_COUNT 16
#define IMAGE_SLICES 64

unsigned long prevTime = 0;
bool endedContact = true;
float currentRPM = 0.0f;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void SetArray(Color array[LED_COUNT]);
void ClearArray();
float GetRPM(unsigned long timeElapsed);
bool DidFullCircle(unsigned long &outTimeElapsed);

void setup()
{
    pinMode(SENSOR_PIN, INPUT);

    strip.begin();
    strip.setBrightness(5);
    strip.show();
}

void loop()
{
    unsigned long timeElapsed = 0;

    if(DidFullCircle(timeElapsed))
    {
        unsigned long columnDelayMicros = timeElapsed / IMAGE_SLICES;
        long actualDelay = (long)columnDelayMicros - 550;
        if(actualDelay < 0)
            actualDelay = 0;

        Color column[COLUMN_COUNT];

        for(int col = COLUMN_COUNT - 1; col >= 0; col--) 
        {
            for(int row = 0; row < 16; row++) 
                column[row] = pixels[row][col];
            
            SetArray(column);
            if(actualDelay > 0)
                delayMicroseconds(actualDelay);
        }

        ClearArray();
    }
}

void SetArray(Color array[LED_COUNT])
{
    for(int i = 0; i < LED_COUNT; i++)
        strip.setPixelColor(i, array[i].r, array[i].g, array[i].b);

    strip.show();
}

void ClearArray()
{
    for(int i = 0; i < LED_COUNT; i++)
        strip.setPixelColor(i, 0, 0, 0);

    strip.show();
}

float GetRPM(unsigned long timeElapsed)
{
    if(timeElapsed == 0)
        return 0;

    return (60.0f * 1000000.0f) / (float)timeElapsed;
}

bool DidFullCircle(unsigned long &outTimeElapsed)
{
    int sensorValue = digitalRead(SENSOR_PIN);

    if(sensorValue == LOW)
    {
        if(endedContact)
        {
            endedContact = false;
            unsigned long now = micros();
            outTimeElapsed = now - prevTime;
            prevTime = now;

            return true;
        }
    }
    else
        endedContact = true;

    return false;
}