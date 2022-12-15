#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include "DHT.h"
#include <ArduinoJson.h>

//Sensors
//#define DHTTYPE DHT11 
#define waterlevelsensor   D6    //Sensor Input
#define waterlevelsensor2  D7    //Sensor Input
#define waterlevelsensor3  D8    //Sensor Input
#define bacasensor  A0    //Sensor Input
//#define waterlevelsensor A0 //Sensor Read

//LCD oled library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//WiFi Status LED
#define wifiLed    D4   //D0

// Update these with temp1s suitable for your network.

const char* ssid = "test123";             //WiFI Name
const char* password = "123123123";       //WiFi Password
const char* mqttServer = "20.20.0.245";
const char* mqttUserName = "";            // MQTT username
const char* mqttPwd = "";                 // MQTT password
const char* clientID = "ESP-32 sensor2";   // client id

#define sub_test "Ping"

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;


void setup_wifi() {
 delay(10);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");

 display.clearDisplay();
 display.setTextSize(1);
 display.setCursor(0,0);
 display.println("Connecting To SSID");
 display.setCursor(0,8);
 display.println(ssid);
 display.setCursor(0,16);
 display.println(password);
 display.display();

  for (int i = 0; i < 10; i++)
  {
    display.setCursor(i*5,24);
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
 display.setCursor(0,0);
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
 while (!client.connected()) {
 if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
      display.println("MQTT connected");
      // ... and resubscribe
      client.subscribe(sub_test);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  //Global Declaration for var catch
  //int OFF = 0; 
  
//  if (strstr(topic, sub_Fan1))
//  {
//    for (int i = 0; i < length; i++) {
//      Serial.print((char)payload[i]);
//    }
//    Serial.println();
//    // Switch on the LED if an 1 was received as first character
//    if ((char)payload[0] == '0') {
//      digitalWrite(RelayPin1, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    } else {
//      digitalWrite(RelayPin1, HIGH);  // Turn the LED off by making the voltage HIGH
//    }    
//  }
//  else
//  {
//    Serial.println("unsubscribed topic");
//  }
}

void pubwaterlevel(){
  int sensor1;
  int sensor2;
  int sensor3;
  
  digitalWrite(waterlevelsensor, HIGH);
  digitalWrite(waterlevelsensor2, LOW);
  digitalWrite(waterlevelsensor3, LOW);
  delay(1000);
  sensor1 = analogRead(bacasensor);
  delay(500);   
//  Serial.print("Sensor1 : ");
//  Serial.println(sensor1);
  
  digitalWrite(waterlevelsensor, LOW);
  digitalWrite(waterlevelsensor2, HIGH);
  digitalWrite(waterlevelsensor3, LOW);
  delay(1000);
  sensor2 = analogRead(bacasensor);
  delay(500);  
//  Serial.print("Sensor2 : ");
//  Serial.println(sensor2);
  
//  digitalWrite(waterlevelsensor, LOW);
//  digitalWrite(waterlevelsensor2, LOW);
//  digitalWrite(waterlevelsensor3, HIGH);
//  sensor3 = analogRead(bacasensor);
//  delay(1000);
//  Serial.print("Sensor3 : ");
//  Serial.println(sensor3);

  //Convert data
  DynamicJsonDocument docsend(1024);

  docsend["twater1"] = sensor1*0.5;
  docsend["twater2"] = sensor2*0.5;
  serializeJson(docsend, Serial);

  //Json fetch data
  String message ="";
  char buffer[256];
  message.toCharArray(buffer, 256);
  serializeJson(docsend, buffer);

  //Json Send data
  client.publish("twater", buffer);
  Serial.println("");
  }

void setup() {
  Serial.begin(115200);

      if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    display.clearDisplay();
  }

  // Display Text
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Starting Device");
  display.display();
  delay(2000);
  display.clearDisplay();
  
  pinMode(waterlevelsensor, OUTPUT);  
  pinMode(waterlevelsensor2, OUTPUT);
  pinMode(waterlevelsensor3, OUTPUT);
  pinMode(bacasensor,INPUT);  
  pinMode(wifiLed, OUTPUT);

  
  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);
  
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  client.loop();
  pubwaterlevel();
 
}
