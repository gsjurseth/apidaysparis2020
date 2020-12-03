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

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;


const char* essid   = "swetx2.4";
const char* wpa     = "thestarsatnight";
const String svcurl = "http://emea-poc15-test.apigee.net/v1/colorsmock";
const String apikey = "cdGxGLFi1JlcbGt5Mg9GHC7eFoT4O7DS";
const int numleds    = 60;

struct Colors {
  int red;
  int blue;
  int green;
}

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(essid, wpa);

}

Colors fetchColors() {
  
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, svcurl)) {  // HTTP
      http.addHeader("apikey", apikey);


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // Parse response
      DynamicJsonDocument doc(2048);

      // httpCode will be negative on error
      if (httpCode > 0) {
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
          int reds = round( numleds * atof(doc["red"]) );
          Serial.println( reds );


          Serial.print( "Green: ");
          int greens = round( numleds * atof(doc["green"]) );
          Serial.println( greens );

          Serial.print( "Blue: ");
          int blues = round( numleds * atof(doc["blue"]) );
          Serial.println( blues );
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  delay(10000);
}
