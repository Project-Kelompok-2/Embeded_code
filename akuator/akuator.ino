/////// AKUATOR CODE FOR WOKSHOP ////////
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
//#include "DHT.h"

//LCD oled library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//Sensors
//#define DHTTYPE DHT11

// Relays
#define RelayPinFan1 D8  //Module Realy
#define RelayPinFan2 D7  //Module Realy
#define RelayPinFan3 D6  //Module Realy
#define RelayPinFan4 D5  //Module Realy

//#define DHTPin1  D5    //Sensor Input
//#define DHTPin12 D6    //Sensor Input


//WiFi Status LED
#define wifiLed D4  //D4

// Update these with values suitable for your network.

const char* ssid = "RESCOM JEMBER";               //WiFI Name
const char* password = "wifirescom222dankos555";  //WiFi Password
const char* mqttServer = "mopgreen.my.id";
const char* mqttUserName = "";              // MQTT username
const char* mqttPwd = "";                   // MQTT password
const char* clientID = "ESP-32 akuator 1";  // client id

//Fan sub variable
#define sub_Fan1 "Fan1"
#define sub_Fan2 "Fan2"
#define sub_Fan3 "Fan3"
#define sub_Fan4 "Fan4"

#define sub_CoolPad1 "CoolPad1"
#define sub_CoolPad2 "servo1"
#define sub_PompaMisting "PompaMisting"
#define sub_FreshWater "FreshWater"
#define sub_PompaN1 "PompaN1"
#define sub_PompaN2 "PompaN2"
#define sub_AutoControl "auto_control"
#define sub_sensor "sensor"
//#define pub_dht11 "tr2/yellow"

//////DISPLAY COLOR//////
////////////////////////

WiFiClient espClient;
PubSubClient client(espClient);


////inisiasi awal nilai variable value
int setFan1 = 0;
int setFan2 = 0;
int setFan3 = 0;
int setFan4 = 0;

int tempValue = 0;
int humValue = 0;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Connecting To SSID");
    display.setCursor(0, 8);
    display.println(ssid);
    display.setCursor(0, 16);
    display.println(password);
    display.display();

    for (int i = 0; i < 10; i++) {
      display.setCursor(i * 5, 24);
      display.println(".");
      Serial.print(".");
      display.display();
      delay(100);
    }
  }
  Serial.println("");
  display.clearDisplay();
  Serial.println("WiFi connected");
  display.display();
  display.setCursor(0, 0);
  display.println("WiFi connected");
  display.display();
  delay(1000);

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.println("IP address: ");
  display.println(WiFi.localIP());
  display.display();
  delay(1000);
}

void reconnect() {
  display.clearDisplay();
  while (!client.connected()) {
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      // ... and resubscribe
      Serial.println("MQTT connected");
      client.subscribe(sub_Fan1);
      client.subscribe(sub_Fan2);
      client.subscribe(sub_Fan3);
      client.subscribe(sub_Fan4);
      client.subscribe(sub_sensor);
      client.subscribe(sub_AutoControl);
      //      client.subscribe(pub_dht11);
    } else {
      display.clearDisplay();
      Serial.print("failed, rc=");
      display.setCursor(0, 0);
      display.println("Connecting to");
      display.setCursor(0, 8);
      display.println("MQTT server");
      display.display();

      for (int i = 0; i < 10; i++) {
        display.setCursor(i * 5, 24);
        display.println(".");
        display.display();
        delay(100);
      }

      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void set_mqtt() {
  if (setFan1 == 1) {
    client.publish(sub_Fan1, "1");
    delay(1000);
  } else if (setFan2 == 1) {
    client.publish(sub_Fan2, "2");
  }
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  //Global Declaration for var catch
  //int OFF = 0;

  if (strstr(topic, sub_Fan1)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(RelayPinFan1, LOW);  // Turn the LED on (Note that LOW is the voltage level

      setFan1 = 1;
      display.clearDisplay();
      display.setCursor(8, 8);
      display.setTextSize(2);
      display.println("FAN 1 ON");
      display.display();
    } else {
      digitalWrite(RelayPinFan1, HIGH);  // Turn the LED off by making the voltage HIGH

      display.clearDisplay();
      display.setCursor(8, 8);
      display.setTextSize(2);
      display.println("FAN 1 OFF");
      display.display();
    }
  }

  else if (strstr(topic, sub_Fan2)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(RelayPinFan2, LOW);  // Turn the LED on (Note that LOW is the voltage level

      display.clearDisplay();
      display.setCursor(8, 8);
      display.setTextSize(2);
      display.println("FAN 2 ON");
      display.display();
    } else {
      digitalWrite(RelayPinFan2, HIGH);  // Turn the LED off by making the voltage HIGH

      display.clearDisplay();
      display.setCursor(8, 8);
      display.setTextSize(2);
      display.println("FAN 2 OFF");
      display.display();
    }
  } else if (strstr(topic, sub_Fan3)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(RelayPinFan3, LOW);  // Turn the LED on (Note that LOW is the voltage level

      display.clearDisplay();
      display.setCursor(8, 8);
      display.setTextSize(2);
      display.println("FAN 3 ON");
      display.display();
    } else {
      digitalWrite(RelayPinFan3, HIGH);  // Turn the LED off by making the voltage HIGH

      display.clearDisplay();
      display.setCursor(8, 8);
      display.setTextSize(2);
      display.println("FAN 3 OFF");
      display.display();
    }
  } else if (strstr(topic, sub_Fan4)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(RelayPinFan4, LOW);  // Turn the LED on (Note that LOW is the voltage level

      display.clearDisplay();
      display.setCursor(8, 8);
      display.setTextSize(2);
      display.println("FAN 4 ON");
      display.display();
    } else {
      digitalWrite(RelayPinFan4, HIGH);  // Turn the LED off by making the voltage HIGH

      display.clearDisplay();
      display.setCursor(8, 8);
      display.setTextSize(2);
      display.println("FAN 4 OFF");
      display.display();
    }
  }

  else if (strstr(topic, sub_sensor)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    DynamicJsonDocument doc(1024);

    // Parse JSON object
    auto error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
    }
    // Decode JSON/Extract values
    int temp = doc["temp1"];
    int humidity = doc["humadity1"];

    tempValue = temp;
    humValue = humidity;

    // Serial.println(tempValue);
    // Serial.print(" ");
    // Serial.println(humValue);

  }

  else if (strstr(topic, sub_AutoControl)) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }

    DynamicJsonDocument doc(1024);

    // Parse JSON object
    auto error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
    }
    // Decode JSON/Extract values
    int data_Satas = doc["suhu_atas"];
    int data_Sbawah = doc["suhu_bawah"];
    int data_Katas = doc["kelembapan_atas"];
    int data_Kbawah = doc["kelembapan_bawah"];

    //  Serial.println("");
    //  Serial.println("");
    //  Serial.println("Hasil Decode");
    //  Serial.print(data_Satas);
    //  Serial.print(" ");
    //  Serial.print(data_Sbawah);
    //  Serial.print(" ");
    //  Serial.print(data_Katas);
    //  Serial.print(" ");
    //  Serial.println(data_Kbawah);


    if (data_Satas >= tempValue) {
      Serial.println("");
      Serial.println("Kipas ON");
      Serial.print(data_Satas);
      Serial.print(">=");
      Serial.println(tempValue);
      // Check Humadity
      if (data_Katas <= humValue) {
        Serial.println("Coolpad ON");
        Serial.print(data_Katas);
        Serial.print("<=");
        Serial.println(humValue);
      } else if (data_Kbawah >= humValue) {
        Serial.println("Coolpad OFF");
        Serial.print(data_Kbawah);
        Serial.print(">=");
        Serial.println(humValue);
      }
    } else if (data_Sbawah <= tempValue) {
      Serial.println("");
      Serial.println("Kipas OFF");
      // Check Humadity
      if (data_Katas <= humValue) {
        Serial.println("Coolpad ON");
        Serial.print(data_Katas);
        Serial.print("<=");
        Serial.println(humValue);
      } else if (data_Kbawah >= humValue) {
        Serial.println("Coolpad OFF");
        Serial.print(data_Kbawah);
        Serial.print(">=");
        Serial.println(humValue);
      }
    }
  }

  else {
    Serial.println("unsubscribed topic");
  }
}

void setup() {
  Serial.begin(115200);
  //  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
    display.clearDisplay();
  }

  // Display Text
  //  display.clearDisplay();
  //  display.setTextSize(1);
  //  display.setTextColor(WHITE);
  //  display.setCursor(0,0);
  //  display.println("Starting Device");
  //  display.display();
  //  delay(2000);
  //  display.clearDisplay();

  pinMode(RelayPinFan1, OUTPUT);
  pinMode(RelayPinFan2, OUTPUT);
  pinMode(RelayPinFan3, OUTPUT);
  pinMode(RelayPinFan4, OUTPUT);

  pinMode(wifiLed, OUTPUT);
  digitalWrite(RelayPinFan1, LOW);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPinFan1, HIGH);
  digitalWrite(RelayPinFan2, HIGH);
  digitalWrite(RelayPinFan3, HIGH);
  digitalWrite(RelayPinFan4, HIGH);

  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);

  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);

  display.clearDisplay();
  display.setCursor(20, 0);
  display.println("Device Ready");
  display.display();
}

void loop() {
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  client.loop();
}
