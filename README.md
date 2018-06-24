# arduino_IoT
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

Ziel war es eine Wasserstandsanzeige aufzubauen die über das Haus WLAn die Werte nach Möglichkeit Webbasiert aufzubereiten um sie auf dem Smartphone oder meinem Magicirror anzeigen zu können. Eine Stromversorgung in der Zisterne war kompliziert deshal habe ich nach einer brauchbaren Batteriebetriebenen Variante gesucht.

Inspiriert haben mich dabei die Überlegungen von Tobias
V1:
https://arduino-projekte.info/zisterne-fuellstandsanzeige-2-0-mit-wemos-lolin32-mysql-datenbank/

V2.0: (das ist das Ziel)
https://arduino-projekte.info/zisterne-fuellstandsanzeige-2-0-mit-wemos-lolin32-mysql-datenbank/

Sensor Erklärung:
https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/

