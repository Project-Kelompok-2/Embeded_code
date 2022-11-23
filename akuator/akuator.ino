#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

//Sensors
#define DHTTYPE DHT11 

// Relays
#define RelayPin1 D1  //Module Realy 
#define RelayPin2 D2  //Module Realy
#define RelayPin3 D3  //Module Realy
#define RelayPin4 D4  //Module Realy 

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


DHT dht(DHTPin1, DHTTYPE);

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
//set msg1
#define MSG_BUFFER_SIZE  (80)
char msg[MSG_BUFFER_SIZE];
//set msg2
#define MSG_BUFFER_SIZE2  (80)
char msg2[MSG_BUFFER_SIZE];
//inisiasi awal nilai variable value
int value = 0;
int value2 = 0;

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
  
  if (strstr(topic, sub_Fan1))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(RelayPin1, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(RelayPin1, HIGH);  // Turn the LED off by making the voltage HIGH
    }    
  }

  else if ( strstr(topic, sub_Fan2))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(RelayPin2, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(RelayPin2, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }
  else if ( strstr(topic, sub_Fan3))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(RelayPin3, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(RelayPin3, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }
  else if (strstr(topic, sub_Fan4))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(DHTPin12, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(DHTPin12, HIGH);  // Turn the LED off by making the voltage HIGH
    }    
  }
  
  else if ( strstr(topic, sub_servo))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
//    // Switch on the LED if an 1 was received as first character
//    if ((char)payload[0] == '0') {
//      digitalWrite(RelayPin4, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    } else {
//      digitalWrite(RelayPin4, HIGH);  // Turn the LED off by making the voltage HIGH
//    }
  }
  
  else
  {
    Serial.println("unsubscribed topic");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(DHTPin1, INPUT);  
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(DHTPin1, INPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);
  
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

//int h = dht.readHumidity();
//// Read temperature as Celsius (the default)
//int t = dht.readTemperature();
//
////send Data sensor
//unsigned long now = millis();
//  if (now - lastMsg > 2000) {
//    lastMsg = now;
//    //value berisi data yang di inisiasi
//    value = t;
//    value2 = h;
//    snprintf (msg, MSG_BUFFER_SIZE, "%ld", value);
//    snprintf (msg2, MSG_BUFFER_SIZE2, "%ld", value2);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
//    Serial.println(msg2);
//    //output topic dan isi topic
//    client.publish("temp", msg);
//    client.publish("humadity", msg2);
//    delay(1000);
//  }
//
//Serial.print("Humidity: ");
//Serial.print(h);
//Serial.println("Temperature: ");
//Serial.print(t);
//delay(1000);

}
