#include "DHT.h"
#include <LWiFi.h>
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302)


char ssid[] = "AndroidAP1044";      // your network SSID (name)
char pass[] = "pdfh6278";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

DHT dht(DHTPIN, DHTTYPE);
const char* server = "api.thingspeak.com";
String apiKey = "0POMKZ9YRYBQRB2Z";
int status = WL_IDLE_STATUS;

WiFiClient client;

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void setup() 
{
 //Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
    }
    Serial.println("Connected to wifi");
    printWifiStatus();
  delay(300);   
  Serial.println("Humidity and temperature\n\n");
  delay(700);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
   if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
   // put your main code here, to run repeatedly:
 
   Serial.println("\nStarting connection to server...");

          if (client.connect(server, 80)) {
            client.print("GET https://api.thingspeak.com/update?api_key=");
            client.print(apiKey+"&field1");
            client.println(t);
            client.println("&field2=60");
            client.println(h);
            delay(10);}
        client.stop();
        delay(5000);
}


