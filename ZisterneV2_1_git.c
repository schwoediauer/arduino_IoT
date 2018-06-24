/*************************************************
Project water reservoir filling level measerement
Wemos LOLIN32
(c) by Juergen Schwoediauer June 2018

 - power consumention optimized
 - Database connection MySql
 - Sensor JSN-SR04T

 based on:
 https://arduino-projekte.info/zisterne-fuellstandsanzeige-2-0-mit-wemos-lolin32-mysql-datenbank/
**************************************************/
#include <SPI.h>
#include <WiFiClientSecure.h>

IPAddress ip(192, 168, 1, 100);
IPAddress dns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);

const char* ssid = "MyWIFI";
const char* password = "MyPassword";
int status = WL_IDLE_STATUS;
const int sleepSeconds = 28.779;

//Webserver, wo die Datei zum befüllen der Datenbank leigt, nur die Adresse (z.B. arduino-projekte.info)
const char* host = "www.myserver.at";
const int httpsPort = 443;

//sensor data
int trigger = 13;
int echo = 12;
long dauer = 0;
long entfernung = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, password);
    // wait 10 seconds for connection:
    delay(10000);
  }

  // print your WiFi shield's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {

  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);
  entfernung = (dauer / 2) / 29.1;

/* Berechnung des ZisternenInhalt nach realem Maß
  if (entfernung >= 200 || entfernung <= 0)
  {
    entfernung = 0;
  }
  else
  {
  entfernung = 192 - entfernung;
  entfernung = entfernung * 84.7457;
  }
*/
  Serial.println("\nStarting connection to server...");
  //HTTPS Server connection
  WiFiClientSecure client;

  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }
    else {

        String url = "/zisterne/writemysql.php";
        url += "?liter=";
        url += entfernung;

        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");

        unsigned long timeout = millis();
        while (client.available() == 0) {
             if (millis() - timeout > 5000) {
                 Serial.println(">>> Client Timeout !");
                 client.stop();
                 return;
               }
         }

        //client.println();  //Verbindungs mit Server aufbauen und HTTP Anfrage senden
//        Serial.println("request sent");

        // Read all the lines of the reply from server and print them to Serial
        while (client.available()) {
          String line = client.readStringUntil('\r');
          Serial.print(line);
        }

    }

//  Serial.println();
  Serial.println("closing connection");
  client.stop();
  //connection close
  delay(10000);

  ESP.deepSleep(sleepSeconds * 30000000); //Wemos Sleep mode
}
