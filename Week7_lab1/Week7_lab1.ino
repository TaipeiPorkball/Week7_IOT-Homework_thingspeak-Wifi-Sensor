#include <LWiFi.h>
#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);


char ssid[] = "AndroidAP1044";      // your network SSID (name)
char pass[] = "pdfh6278";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
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
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
  Serial.println("Temperature and Humidity");
  dht.begin();
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          client.println("<head>\n");
          client.println("<meta charset='UTF-8'/>\n");
          client.println("<title>Temperature and Humidity</title>\n");
          client.println("<style>");
          client.println( "html{background-color:#c5e9dc;}");
          client.println(".title{text-align:center;color:darkblue;}");
          client.println("a{display:block;text-decoration:none;background-color:lightgray;");
          client.println("width:150px;text-align:center;margin:0 auto;margin-bottom:30px;font-size:2em;}");
          client.println("btn1{background-color:black;color:white;}");
          client.println("btn2{background-color:white;color:black;}");
          client.println("</style>");
          client.println("</head>\n");
          client.println("<body>");
          client.println("<h1 class=\"title\">Temperature and Humidity</h1>");
          client.print("<a class=\"btn1\" href=\"/H\">Temperature</a>");
          client.println(t);
          client.print("<a class=\"btn2\" href=\"/L\">Humidity</a>");
          client.println(h);
          client.println("</body></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1000);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


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
