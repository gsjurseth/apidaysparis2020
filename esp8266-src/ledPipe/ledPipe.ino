/**
   Used BasicHTTPClient.ino as a base
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <math.h>
#include <FastLED.h>

/*
 * Local secrets file where I #define ESSID, WPA, SVCURL (for the apigee proxy), and APIKEY (for auth with the apigee proxy)
 */
#include "./secrets.h"

#define LED_PIN    4
#define NUM_LEDS 60
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100


ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;

struct Colors {
  int red;
  int blue;
  int green;
};

int brightness = 10;
int run = 0;
Colors prevclrs;
Colors myclrs;



void showStrip() {
  #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    strip.show();
  #endif
  #ifndef ADAFRUIT_NEOPIXEL_H
    // FastLED
    FastLED.show();
  #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void setColor(int start, int length, char color[]) {
  Serial.print("Entered setColor with color: ");
  Serial.print(color);
  Serial.print(" and with start ");
  Serial.print(start);
  Serial.print(" and with length: ");
  Serial.println(length);

  if ( strcmp(color, "red") == 0) {
    Serial.print("setting red for: ");
    Serial.println(color);
    for (int i = start; i < (start+length); i++) {
      leds[i] = CRGB::Red; FastLED.show();
    }
    FastLED.show();
  }
  else if ( strcmp(color, "green") == 0) {
    Serial.print("setting green for: ");
    Serial.println(color);
    for (int i = start; i < (start+length); i++) {
      leds[i] = CRGB::Green; FastLED.show();
    }
    FastLED.show();
  }
  else {
    Serial.print("setting blue for: ");
    Serial.println(color);
    for (int i = start; i < (start+length); i++) {
      leds[i] = CRGB::Blue; FastLED.show();
    }
    FastLED.show();
    
  }
}

void setWhite(int start, int length) {
  for (int i = start; i < (start+length); i++) {
    leds[i] = CRGB::White; FastLED.show();
  }
  FastLED.show();
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);
}

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  //FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(brightness);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ESSID, WPA);

  setWhite(0, 60);
}

Colors fetchColors() {
  Colors clrs;
  
  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, SVCURL)) {  // HTTP
    http.addHeader("apikey", APIKEY);


    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // Parse response
    DynamicJsonDocument doc(2048);

    // httpCode will be negative on error
    if (httpCode == 200) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);

        // read the vals
        deserializeJson(doc, payload);

        // print values
        Serial.print( "Red: ");
        clrs.red = round( NUM_LEDS * atof(doc["red"]) );
        Serial.println( clrs.red );


        Serial.print( "Green: ");
        clrs.green = round( NUM_LEDS * atof(doc["green"]) );
        Serial.println( clrs.green );

        Serial.print( "Blue: ");
        clrs.blue = round( NUM_LEDS * atof(doc["blue"]) );
        Serial.println( clrs.blue );
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
  return clrs;
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    
    if (run = 1) {
      prevclrs.red = myclrs.red;
      prevclrs.blue = myclrs.blue;
      prevclrs.green = myclrs.green;
    }

    myclrs = fetchColors();
    if (( myclrs.red == prevclrs.red) && (myclrs.blue == prevclrs.blue) && (myclrs.green == prevclrs.green)) {
      Serial.println("Looks like there's been no change");  
    }
    else {
      CylonBounce(0xff, 0, 0, 4, 10, 50);    
    }



    if (myclrs.red > 61) {
      setWhite(0,60);
    }
    else {
      int i = 0;
      setColor(i, myclrs.red, "red");
      i = i + myclrs.red;
      setColor(i, myclrs.green, "green");
      i = i + myclrs.green;
      setColor(i, myclrs.blue, "blue");
    }
    
    run = 1;
  }
  delay(3000);
}
