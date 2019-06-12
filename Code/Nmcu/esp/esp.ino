#include <ESP8266WiFi.h>

String result;
char host[]="script.google.com";
WiFiClient client;
void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
WiFi.begin("vishnu", "qwertyuiop");
while(WiFi.status() != WL_CONNECTED){
Serial.print(".");
delay(500);
}
Serial.println("Connected!");
Serial.println(WiFi.localIP());
}
void loop() {
Time();
delay(3000);
}
String url = "/macros/s/AKfycbyd5AcbAnWi2Yn0xhFRbyzS4qMq1VucMVgVvhul5XqS9HkAyJY/exec";
void Time(){
if (client.connect(host, 443)) { //starts client connection, checks for connection
client.print(String("GET ") + url + " HTTP/1.1\r\n" +
"Host: " + host + "\r\n" +
"Connection: close\r\n\r\n");
Serial.println("Server is accessible");
} else {
Serial.println("connection failed"); //error message if no client connect
Serial.println();
}
result = "";
while (client.available()) { //connected or data available
char c = client.read(); //gets byte from ethernet buffer
result = result+c;
}
Serial.println(result);
client.stop(); //stop client
Serial.println("end of function");
}
