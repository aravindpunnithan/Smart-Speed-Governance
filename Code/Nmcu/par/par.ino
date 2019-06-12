//http://playerservices.streamtheworld.com/api/livestream?version=1.5&mount=CIMXFMAAC&lang=en
#include <ESP8266WiFi.h>
#include <TinyXML.h>

const char* ssid     = "SSID";
const char* password = "PASSWORD";

const char* host = "playerservices.streamtheworld.com";
const char* apiVersion   = "1.5";
const char* mountPoint = "CIMXFMAAC";
const char* lang = "en";

// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80;
TinyXML xml;
uint8_t    buffer[150]; // For XML decoding
//uint16_t buflen = 150;
String xmlOpen;
String xmlTag;
String xmlData;
String xmlAttrib;
String xmlDataLast;
// UTILITY FUNCTIONS -------------------------------------------------------

// TinyXML invokes this callback as elements are parsed in XML,
// allowing us to pick out just the tidbits of interest rather than
// dynamically allocating and then traversing a whole brobdingnagian
// tree structure; very Arduino-friendly!
// As written here, this looks for XML tags named "seconds" and extracts
// the accompanying value (a predicted bus arrival time, in seconds).
// If it's longer than our minimum threshold, the least two values are
// sorted and saved (sooner time first); one or both may be zero if
// further predictions are not available. Results are stored in global
// two-element int array named 'seconds' and moved to stop struct later.
void XML_callback( uint8_t statusflags, char* tagName,  uint16_t tagNameLen,  char* data,  uint16_t dataLen )
{
  if (statusflags & STATUS_START_TAG)
  {
    if ( tagNameLen )
    {
      xmlOpen = String(tagName);
      //Serial.print("Start tag ");
      //Serial.println(tagName);
    }
  }
  else if  (statusflags & STATUS_END_TAG)
  {
    //Serial.print("End tag ");
    //Serial.println(tagName);
  }
  else if  (statusflags & STATUS_TAG_TEXT)
  {
    xmlTag = String(tagName);
    xmlData = String(data);
    //Serial.print("Tag:");
    //Serial.print(tagName);
    //Serial.print(" text:");
    //Serial.println(data);
  }
  else if  (statusflags & STATUS_ATTR_TEXT)
  {
    //Serial.print("Attribute:");
    //Serial.print(tagName);
    //Serial.print(" text:");
    //Serial.println(data);
  }
  else if  (statusflags & STATUS_ERROR)
  {
    //Serial.print("XML Parsing error  Tag:");
    //Serial.print(tagName);
    //Serial.print(" text:");
    //Serial.println(data);
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("");
  Serial.print("WiFi connecting to ");  
  Serial.print(ssid);
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  xml.init((uint8_t *)buffer, sizeof(buffer), &XML_callback);
  //xml.init((uint8_t*)&buffer,buflen,&XML_callback);
  // Connect to a WiFi network
  Serial.println("");
  Serial.println("WiFi connected.");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

//void wifiConnect() {
  //int      c;
  //uint32_t t;

  
  /*
    for(c=0; (c < 60) && (WiFi.status() != WL_CONNECTED); c++) {
      Serial.write('.');
      for(t = millis(); (millis() - t) < 1000;);
    }
    if(c >= 60) { // If it didn't connect within 1 min
      Serial.println("Failed. Will retry...");
      return;
    }
    */
/*
if (WiFi.status() != WL_CONNECTED) { // FIX FOR USING 2.3.0 CORE (only .begin if not connected)
    WiFi.begin(ssid, password); // connect to the network
}
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
    Serial.println("WiFi connected");  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    //delay(2000); // Pause before hitting it with queries & stuff.
  }
}
*/
String stationServer("");
String stationPort("");
String stationMount("");

void loop() {
 // wifiConnect();
  Serial.print("Connecting to url ");
  //http://playerservices.streamtheworld.com/api/livestream?version=1.5&mount=CIMXFMAAC&lang=en
  // We now create a URI for the request
  String url = "/api/";
  url += "livestream";
  url += "?version=";
  url += apiVersion;
  url += "&mount=";
  url += mountPoint;
  url += "&lang=";
  url += lang;
  Serial.print("http://");
  Serial.print(host);
  Serial.print(":");
  Serial.print(httpPort);
  Serial.println(url);
  if (client.connect(host, httpPort)) {
    Serial.println("Connected");
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n");
    client.println();
    client.println();
    while (true) {
      if (client.available()) {
        char c = client.read();
        //xml.processChar(c);
        //Serial.print(c);
        if (c=='<') {
          c=client.read();
          if (c=='?') {
            xml.processChar('<');
            xml.processChar('?');
            break;
          }
        }
      }
    }
    Serial.println("Waiting for response");
    //if (client.available()) {
    while (true) {
      if (client.available()) {
        char c = client.read();
        xml.processChar(c);
        if (xmlTag != "") {
          if (xmlTag.endsWith(F("/ip"))) {
            stationServer = xmlData;
            //Serial.println(stationServer);
          }else if (xmlTag.endsWith(F("/port"))) {
            stationPort = xmlData;
            //Serial.println(stationPort);
          }else if (xmlTag.endsWith(F("/mount"))) {
            stationMount = xmlData;
            //Serial.println(stationMount);
          }
        }
        //xml.reset();
        //Serial.print(c);
      }
      if (stationServer != "" && stationPort != "" && stationMount != "") {
        break;
      }
    }
           //delay(1000);
            Serial.print("http://");
            Serial.print(stationServer);
            Serial.print(":");
            Serial.print(stationPort);
            Serial.print("/");
            Serial.print(stationMount);
            Serial.println("_CS");
            xmlDataLast = xmlData;
  }

  //if (Serial.available()) {
    //xml.processChar(Serial.read());
  //}

   
  Serial.println("closing connection");
}

/*
struct {
  const uint8_t     addr;          // I2C address of display
  const char       *agency;        // Get these using routefinder.py script
  const char       *route;
  const char       *stopID;
  uint32_t          lastQueryTime; // Time of last query (millis())
  uint32_t          seconds[2];    // Most recent predictions from server
//  Adafruit_7segment display;       // 7segment initialized at runtime
} stops[] = {
  { 0x70, "actransit", "210", "0702640" }, // Ohlone College
  { 0x71, "actransit", "210", "0702630" }, // Union Landing
  { 0x72, "actransit", "232", "0704440" }, // Fremont BART
  { 0x73, "actransit", "232", "0704430" }  // NewPark Mall
};
*/

/*
 * 
 *     //client.print(String("GET ") + url + " HTTP/1.1\r\n" +
    //           "Host: " + host + "\r\n" + 
    //           "Connection: close\r\n\r\n");
        //client.println("HOST: ") + host;
    //client.print("Authorization: Basic ");
    //client.println(mime);
    //client.println("User-Agent: Arduino HQ");
  Serial.print("connecting to ");
  Serial.println(host);
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  //http://playerservices.streamtheworld.com/api/livestream?version=1.5&mount=CIMXFMAAC&lang=en
  // We now create a URI for the request
  String url = "/api/";
  url += "livestream";
  url += "?version=";
  url += apiVersion;
  url += "&mount=";
  url += mountPoint;
  url += "&lang=";
  url += lang;
  //url += "?private_key=";
  //url += privateKey;
  //url += "&value=";
  //url += value;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  client.flush();
  xml.reset();
  //memset(seconds, 0, sizeof(seconds)); // Clear predictions
  t        = millis(); // Start time
  timedOut = false;
  while(client.connected()) {
    if((c = client.read()) >= 0) {
      xml.processChar(c);
      if (xmlTag != "") {
        if (xmlTag.endsWith(F("/ip"))) {
          stationPort = xmlData;
          Serial.println(stationServer);
        //} else if (xmlTag.endsWith(F("/ip"))) {
          //serverIP = xmlData.toInt();
        } else if (xmlTag.endsWith(F("/port"))) {
          stationPort = xmlData;
          Serial.println(stationPort);
        } else if (xmlTag.endsWith(F("/mount"))) {
          stationMount = xmlData;
          Serial.println(stationMount);
        }
        
            /*
             else if (xmlTag.endsWith(F("/ssidpass"))) {
              password = xmlData; 
            } else if (xmlTag.endsWith(F("/ip"))) {
              ipIsOk = ip.fromString(xmlData);
            } else if (xmlTag.endsWith(F("/mask"))) {
              mask.fromString(xmlData);
            } else if (xmlTag.endsWith(F("/gw"))) {
              gw.fromString(xmlData);
            } else if (xmlTag.endsWith(F("/dns"))) {
              ns.fromString(xmlData);
            } else if (xmlTag.endsWith(F("/syslog"))) {
              sysLogServer.fromString(xmlData);
            } else if (xmlTag.endsWith(F("/name"))) {
              name = xmlData;
            } else if (xmlTag.endsWith(F("/pin1wire"))) {
              pinOneWire = xmlData.toInt();
            } else if (xmlTag.endsWith(F("/partner")) || xmlTag.endsWith(F("/pull"))) {
              for (i = 0; i < PARTNER_MAX_COUNT; i++) {
                if (pull[i] == "") {
                  pull[i] = xmlData;
                  break;
                }
              }
            } else if (xmlTag.endsWith(F("/push"))) {
              for (i = 0; i < PARTNER_MAX_COUNT; i++) {
                if (push[i] == "") {
                  push[i] = xmlData;
                  break;
                }
              }
            } else if (xmlTag.endsWith(F("/allow"))) {
              for (i = 0; i < PARTNER_MAX_COUNT; i++) {
                if (allow[i] == "") {
                  allow[i] = xmlData;
                  break;
                }
              }
            } else if (xmlTag.endsWith(F("/feature/sensors"))) {
              use.sensors = (xmlData.toInt() == 1);
            } else if (xmlTag.endsWith(F("/feature/lcd"))) {
              use.lcd = (xmlData.toInt() == 1);
            } else if (xmlTag.endsWith(F("/feature/heater"))) {
              use.heater = (xmlData.toInt() == 1);
            } else if (xmlTag.endsWith(F("/feature/partners"))) {
              use.partners = (xmlData.toInt() == 1);
            } else if (xmlTag.endsWith(F("/feature/ap"))) {
              use.ap = (xmlData.toInt() == 1);
            } else if (xmlTag.endsWith(F("/feature/syslog"))) {
            //Not implemented
            
        }
        xmlTag = "";
        xmlData = "";
        yield();
        t = millis(); // Reset timeout clock
      } else if((millis() - t) >= (READ_TIMEOUT * 1000)) {
        Serial.println("---Timeout---");
        timedOut = true;
        break;
      }
      //if(!timedOut && seconds[0]) { // Successful transfer?
        // Copy newly-polled predictions to stops structure:
        //memcpy(stops[s].seconds, seconds, sizeof(seconds));
        //stops[s].lastQueryTime = millis();
      //}
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
      client.stop();
      Serial.println();
  /*
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 50000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
 
  // Read all the lines of the reply from server and print them to Serial
  //while(client.available()){
  //  String line = client.readStringUntil('\r');
  //  Serial.print(line);
  //}
  //stationMount */
