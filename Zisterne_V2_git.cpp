/*************************************************
Project water reservoir filling level measurement
Wemos LOLIN32
(c) by Juergen Schwoediauer July 2018

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

const char* ssid = "MySID";
const char* password = "MySIDPassword";
const int sleepMilliSeconds = 3000000000; //~71min.
const int maxDistance = 208;
const long literFact = 49.0875;
int resetCount = 0;
int status = WL_IDLE_STATUS;

//Database server adress (e.g. arduino-projekte.info)
const char* host = "www.mywebserver.com";
const int httpsPort = 443;

//sensor data
int trigger = 13;
int echo = 12;
long sensDuration = 0;
long sensDistance = 0;

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
    resetCount++;
    if (resetCount >= 15){
      Serial.print("Interrupt wait for WLAN...");
      ESP.deepSleep(sleepMilliSeconds); //wait for WLAN
    }

  }

  // print your WiFi shield's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  //Serial.println(ESP.deepSleep(ESP.deepSleepMax()));

}

void loop() {

  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  sensDuration = pulseIn(echo, HIGH);
  sensDistance = (sensDuration / 2) / 29.1;

  //calculation cistern volume in liter
  //set range from 0 to 100%
  if (sensDistance >= maxDistance || sensDistance <= 0)
  {
    if (sensDistance >= maxDistance) {
      sensDistance = maxDistance;
    } else {
      sensDistance = 0;
    }
  }

  sensDistance = maxDistance - sensDistance;
  sensDistance = sensDistance * literFact;
  Serial.print("Zisterneninhalt = ");
  Serial.println(sensDistance);

  Serial.println("\nStarting connection to server...");
  //HTTPS Server connection
  WiFiClientSecure client;

  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      Serial.print("Interrupt wait for WLAN...");
      ESP.deepSleep(sleepMilliSeconds); //wait for WLAN
      return;
    }
    else {

        String url = "/zisterne/writemysql.php";
        url += "?liter=";
        url += sensDistance;

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

  ESP.deepSleep(sleepMilliSeconds); //Wemos Sleep mode
}
