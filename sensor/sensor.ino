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


//WiFi Status LED
#define wifiLed    D0   //D0

// Update these with temp1s suitable for your network.

const char* ssid = "test123"; //WiFI Name
const char* password = "123123123"; //WiFi Password
const char* mqttServer = "20.20.0.245";
const char* mqttUserName = ""; // MQTT username
const char* mqttPwd = ""; // MQTT password
const char* clientID = "ESP-32 sensor"; // client id

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
///sensor1
int temp1 = 0;
int humadity1 = 0;
///sensor2
int temp2 = 0;
int humadity2 = 0;

void setup_wifi() {
 delay(10);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}

void reconnect() {
 while (!client.connected()) {
 if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
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
  dht.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  pinMode(DHTPin1, INPUT);  
  pinMode(DHTPin2, INPUT);
  pinMode(DHTPin3, INPUT);  
  pinMode(DHTPin4, INPUT);
  pinMode(wifiLed, OUTPUT);


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
    humadity1 = h1*1.81;
    delay(100);
    temp2 = t2;
    humadity2 = h2;
    
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
    delay(5000);
  }
Serial.println("");
Serial.print("Humidity1: ");
Serial.println(h1*1.81);  //
Serial.print("Temperature1: ");
Serial.println(t1);
Serial.print("Humidity2: ");
Serial.println(h2);  //
Serial.print("Temperature2: ");
Serial.println(t2);
Serial.print("Humidity3: ");
Serial.println(h3*0.90);  //
Serial.print("Temperature3: ");
Serial.println(t3);
Serial.print("Humidity4: ");
Serial.println(h4*2.10);  //
Serial.print("Temperature4: ");
Serial.println(t4);
  }


void loop() {
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  client.loop();
  
pubsen1();
}
