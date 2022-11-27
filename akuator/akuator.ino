                    /////// AKUATOR CODE FOR WOKSHOP ////////
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include "DHT.h"

//Sensors
//#define DHTTYPE DHT11 

// Relays
#define RelayPinFan1 D8  //Module Realy 
#define RelayPinFan2 D7  //Module Realy
#define RelayPinFan3 D6  //Module Realy
#define RelayPinFan4 D5  //Module Realy 

#define DHTPin1  D5    //Sensor Input
#define DHTPin12 D6    //Sensor Input


//WiFi Status LED
#define wifiLed    D0   //D0

// Update these with values suitable for your network.

const char* ssid = "test123"; //WiFI Name
const char* password = "123123123"; //WiFi Password
const char* mqttServer = "20.20.0.245";
const char* mqttUserName = ""; // MQTT username
const char* mqttPwd = ""; // MQTT password
const char* clientID = "ESP-32 akuator"; // client id


//DHT dht(DHTPin1, DHTTYPE);

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
//#define pub_dht11 "tr2/yellow"



WiFiClient espClient;
PubSubClient client(espClient);

//For Publish
//unsigned long lastMsg = 0;
////set msg1
//#define MSG_BUFFER_SIZE  (80)
//char msg[MSG_BUFFER_SIZE];
////set msg2
//#define MSG_BUFFER_SIZE2  (80)
//char msg2[MSG_BUFFER_SIZE];
////inisiasi awal nilai variable value
//int value = 0;
//int value2 = 0;

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
      client.subscribe(sub_CoolPad1);
      client.subscribe(sub_CoolPad2);
//      client.subscribe(pub_dht11);
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
  
  if (strstr(topic, sub_Fan1))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(RelayPinFan1, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(RelayPinFan1, HIGH);  // Turn the LED off by making the voltage HIGH
    }    
  }

  else if ( strstr(topic, sub_Fan2))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(RelayPinFan2, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(RelayPinFan2, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }
  else if ( strstr(topic, sub_Fan3))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(RelayPinFan3, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(RelayPinFan3, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }
  else if (strstr(topic, sub_Fan4))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(RelayPinFan4, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(RelayPinFan4, HIGH);  // Turn the LED off by making the voltage HIGH
    }    
  }
  
//  else if ( strstr(topic, sub_CoolPad2))
//  {
//    for (int i = 0; i < length; i++) {
//      Serial.print((char)payload[i]);
//    }
//    Serial.println();
//    // Switch on the LED if an 1 was received as first character
//    if ((char)payload[0] == '1') {
//      digitalWrite(RelayPinFan4, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    } else {
//      digitalWrite(RelayPinFan4, HIGH);  // Turn the LED off by making the voltage HIGH
//    }
//  }
  
  else
  {
    Serial.println("unsubscribed topic");
  }
}

void setup() {
  Serial.begin(115200);
//  dht.begin();
  
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
}

void loop() {
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  client.loop();
}
