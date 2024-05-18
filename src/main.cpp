#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <credential.h>

// LED things
#define NUM_LEDS 120
#define DATA_PIN D13
CRGB leds[NUM_LEDS];

// Net/web things
ESP8266WebServer server(80);

void handleRoot();
void handleNotFound();
void handleLED();
void setWholeColor(int color);
void setAllLeds(CRGB color);

void setup()
{
    Serial.begin(9600);
    server.on("/", handleRoot); // Call the 'handleRoot' function when a client requests URI "/"3
    server.on("/led", handleLED);
    server.onNotFound(handleNotFound); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println(WIFI_SSID);
    Serial.println(WIFI_PASSWORD);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    server.begin(); // Actually start the server
    Serial.println("HTTP server started");
    Serial.println("LED strip init to white");
    setAllLeds(CRGB::White);
}

void loop()
{
    server.handleClient();
}

void handleRoot()
{
    server.send(200, "text/plain", "Hello World!");
    Serial.println("Hello World!");
}

static unsigned char nibble(char c)
{
    return c % 16 + 9 * (c >> 6);
}

unsigned int hex2num(const char *s)
{
    unsigned int r = 0;
    while (*s)
        r = (r << 4) | nibble(*s++);
    return r;
}

void handleLED()
{
    String color = server.arg("color");
    Serial.println(color);
    int color_int = hex2num(color.c_str());
    setAllLeds(color_int);
    server.send(200, "text/plain", "OK");
}

void setAllLeds(CRGB color)
{
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
}

// Fade in to white
void fadeIn()
{

}

void handleNotFound()
{
}