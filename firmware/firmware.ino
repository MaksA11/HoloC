#include <Adafruit_NeoPixel.h>

#define SENSOR_PIN 3
#define LED_PIN 4

#define LED_COUNT 16

unsigned long prevTime = 0;
bool endedContact = true;

float currentRPM = 0.0f;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

struct Color
{
    uint8_t r, g, b;

    Color() : r(255), g(255), b(255) {};
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};
};


void SetArray(Color array[LED_COUNT]);
void ClearArray();
float GetRPM(unsigned long timeElapsed);
bool DidFullCircle(unsigned long &outTimeElapsed);

Color red = Color(255, 0, 0);
Color green = Color(0, 255, 0);
Color blue = Color(0, 0, 255);
Color yellow = Color(255, 255, 0);
Color cyan = Color(0, 255, 255);
Color magenta = Color(255, 0, 255);
Color white = Color(255, 255, 255);
Color black = Color(0, 0, 0);
Color orange = Color(255, 165, 0);
Color purple = Color(128, 0, 128);
Color pink = Color(255, 192, 203);
Color gray = Color(128, 128, 128);

Color pixels[16][16] = {
    {black, black, black,   black, black, black, black, yellow, green, yellow, black, black, black,     black, black, black},
    {black, black, black,   black, black, black, yellow, yellow, green, yellow, black, black, black,     black, black, black},
    {black, black, black,   black, yellow, yellow, yellow, green, green, yellow, black, black, black,     black, black, black},
    {black, black, black,   yellow, yellow, green, green, yellow, green, yellow, black, black, black,     black, black, black},
    {black, black, black,   yellow, green, green, yellow, yellow, green, yellow, black, black, black,     black, black, black},
    {black, black, black,   yellow, yellow, green, green, yellow, green, yellow, black, black, black,     black, black, black},
    {black, black, black,   yellow, green, green, yellow, yellow, green, yellow, black, black, black,     black, black, black},
    {black, black, black,   yellow, yellow, green, green, yellow, green, yellow, black, black, black,     black, black, black},
    {black, black, black,   black, yellow, yellow, yellow, green, green, yellow, yellow, black, black,     black, black, black},
    {black, black, black,   black, black, black, yellow, yellow, green, green, yellow, yellow, black,     black, black, black},
    {black, black, black,   black, black, black, black, yellow, green, yellow, green, yellow, yellow,     black, black, black},
    {black, black, black,   black, black, black, black, yellow, green, yellow, yellow, green, yellow,     black, black, black},
    {black, black, black,   black, black, black, black, yellow, green, green, green, yellow, yellow,     black, black, black},
    {black, black, black,   black, black, black, black, yellow, green, yellow, green, yellow, yellow,     black, black, black},
    {black, black, black,   black, black, black, black, yellow, green, yellow, yellow, green, yellow,     black, black, black},
    {black, black, black,   black, black, black, black, yellow, green, yellow, yellow, yellow, yellow,     black, black, black}
};

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
        unsigned long columnDelayMicros = timeElapsed / 64;
        long actualDelay = (long)columnDelayMicros - 550;
        if(actualDelay < 0)
            actualDelay = 0;

        Color column[16];

        for(int col = 15; col >= 0; col--) 
        {
            for(int row = 0; row < 16; row++) 
                column[row] = pixels[row][col];
            
            SetArray(column);
            if(actualDelay > 0)
                delayMicroseconds(actualDelay);
        }

        ClearArray();
    }

    // Color column[16];
    // for(int col = 15; col >= 0; col--) 
    // {
    //     for(int row = 0; row < 16; row++) 
    //         column[row] = pixels[row][col];
    //     SetArray(column);
    //     delay(100);
    // }
    // ClearArray();
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