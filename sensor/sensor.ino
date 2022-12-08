#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <ArduinoJson.h>

//Sensors
#define DHTTYPE DHT11 
#define DHTPin1  D5    //Sensor Input
#define DHTPin2  D6    //Sensor Input
#define DHTPin3  D7    //Sensor Input
#define DHTPin4  D8    //Sensor Input
#define waterlevelsensor A0 //Sensor Read

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

const char* ssid = "SKK - STUDENT";             //WiFI Name
const char* password = "sistemkomputerkontrol";       //WiFi Password
const char* mqttServer = "10.10.0.167";
const char* mqttUserName = "";            // MQTT username
const char* mqttPwd = "";                 // MQTT password
const char* clientID = "ESP-32 sensor";   // client id

//deklarasi pin dht sensor
DHT dht(DHTPin1, DHTTYPE);
DHT dht2(DHTPin2, DHTTYPE);
DHT dht3(DHTPin3, DHTTYPE);
DHT dht4(DHTPin4, DHTTYPE);


#define sub_Fan1 "Fan1"
#define sub_Fan2 "Fan2"
#define sub_Fan3 "Fan3"
#define sub_Fan4 "swF4"
#define pub_dht11 "tr2/yellow"
#define sub_tr2_green "tr2/green"
#define sub_servo "servo1"


WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

//inisiasi awal nilai variable temp1
///dht 11 sensor1
int temp1 = 0;
int humadity1 = 0;
///sensor2
int temp2 = 0;
int humadity2 = 0;

//water level sensor
int sensor_value = 0;

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
      client.subscribe(sub_Fan1);
      client.subscribe(sub_Fan2);
      client.subscribe(sub_Fan3);
      client.subscribe(sub_Fan4);
      client.subscribe(pub_dht11);
      client.subscribe(sub_tr2_green);
      client.subscribe(sub_servo);
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
  display.println("Hello world!");
  display.display();
  delay(2000);
  display.clearDisplay();
  
  dht.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  pinMode(DHTPin1, INPUT);  
  pinMode(DHTPin2, INPUT);
  pinMode(DHTPin3, INPUT);  
  pinMode(DHTPin4, INPUT);
  pinMode(wifiLed, OUTPUT);
  pinMode(waterlevelsensor, INPUT);

  
  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);
  
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void pubsen1(){
////////////READ DATA SENSOR 1
int h1 = dht.readHumidity();
int t1 = dht.readTemperature();
////////////READ DATA SENSOR 2
delay(100);
int h2 = dht2.readHumidity();
int t2 = dht2.readTemperature();
////////////READ DATA SENSOR 2
delay(100);
int h3 = dht3.readHumidity();
int t3 = dht3.readTemperature();
////////////READ DATA SENSOR 2
delay(100);
int h4 = dht4.readHumidity();
int t4 = dht4.readTemperature();


//send Data sensor
unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    //value berisi data yang di inisiasi
    DynamicJsonDocument doc(1024);
    temp1 = t1;
    humadity1 = h1;
    delay(100);
    temp2 = t2;
    humadity2 = h2;
//    temp3 = t3;
//    humadity3 = h3;
//    delay(100);
//    temp1 = t4;
//    humadity1 = h4;
//    delay(100);
    
    //Json fill data here
    doc["temp1"] = temp1;
    doc["humadity1"] = humadity1;
    doc["temp2"] = temp2;
    doc["humadity2"] = humadity2;
    serializeJson(doc, Serial);

    //Json fetch data
    String message ="";
    char buffer[256];
    message.toCharArray(buffer, 256);
    serializeJson(doc, buffer);

    //Json Send data
    client.publish("sensor", buffer);
  }
Serial.println("");

display.setCursor(0,0);
Serial.print("Hum1: ");
Serial.println(humadity1); 
Serial.print("Temp1: ");
Serial.println(temp1);
Serial.print("Hum2: ");
Serial.println(h2);  //
Serial.print("Temp2: ");
Serial.println(t2);
Serial.print("Hum3: ");
Serial.println(h3*0.90);  //
Serial.print("Temp3: ");
Serial.println(t3);
Serial.print("Hum4: ");
Serial.println(h4*2.10);  //
Serial.print("Temp4: ");
Serial.println(t4);

///Display print sensor
delay(1000);
display.clearDisplay();
display.setCursor(0,0);
display.setTextSize(1);
display.print("Hum1: ");
display.print(humadity1); 
display.setCursor(70,0);
display.print("Temp1: ");
display.println(temp1);

delay(500);
display.setCursor(0,8);
display.setTextSize(1);
display.print("Hum2: ");
display.print(humadity2); 
display.setCursor(70,8);
display.print("Temp2: ");
display.println(temp2);
display.display();

delay(500);
display.setCursor(0,16);
display.setTextSize(1);
display.print("Hum3: ");
display.print(h3); 
display.setCursor(70,16);
display.print("Temp3: ");
display.println(t3);
display.display();

delay(500);
display.setCursor(0,24);
display.setTextSize(1);
display.print("Hum4: ");
display.print(h3); 
display.setCursor(70,24);
display.print("Temp4: ");
display.println(t3);
display.display();
delay(500);

display.setCursor(0,48);
display.setTextSize(1);
display.print("Data Sended");
display.display();
delay(2000);
  }

void pubwaterlevel(){
  sensor_value = analogRead(waterlevelsensor);
  Serial.print("Sensor Value = ");
  Serial.println(sensor_value);

  //Convert data
  DynamicJsonDocument docsend(1024);

  docsend["waterLevel"] = sensor_value;
  serializeJson(docsend, Serial);

  //Json fetch data
  String message ="";
  char buffer[256];
  message.toCharArray(buffer, 256);
  serializeJson(docsend, buffer);

  //Json Send data
  client.publish("water_level", buffer);
  delay(5000);
  }

void loop() {
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  client.loop();
  
pubsen1();
//pubwaterlevel();
 
}
