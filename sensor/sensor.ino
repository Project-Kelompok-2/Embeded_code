#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <ArduinoJson.h>

//Sensors
#define DHTTYPE DHT11 

// Relays
//#define RelayPin1 D1  //Module Realy 
//#define RelayPin2 D2  //Module Realy
//#define RelayPin3 D3  //Module Realy
//#define RelayPin4 D4  //Module Realy 

#define DHTPin1  D1    //Sensor Input
#define DHTPin2  D4    //Sensor Input


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

////////////set msg sensor 1///////
//set msg1
#define MSG_BUFFER_SIZE  (80)
char msg[MSG_BUFFER_SIZE];


//set msg2
#define MSG_BUFFER_SIZE2  (80)
char msg2[MSG_BUFFER_SIZE];
///////////////////////////////////

////////////set msg sensor 2///////
//set msg1
#define MSG_BUFFER_SIZE3  (80)
char msg3[MSG_BUFFER_SIZE];


//set msg2
#define MSG_BUFFER_SIZE4  (80)
char msg4[MSG_BUFFER_SIZE];
///////////////////////////////////

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
//
//  else if ( strstr(topic, sub_Fan2))
//  {
//    for (int i = 0; i < length; i++) {
//      Serial.print((char)payload[i]);
//    }
//    Serial.println();
//    // Switch on the LED if an 1 was received as first character
//    if ((char)payload[0] == '0') {
//      digitalWrite(RelayPin2, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    } else {
//      digitalWrite(RelayPin2, HIGH);  // Turn the LED off by making the voltage HIGH
//    }
//  }
//  else if ( strstr(topic, sub_Fan3))
//  {
//    for (int i = 0; i < length; i++) {
//      Serial.print((char)payload[i]);
//    }
//    Serial.println();
//    // Switch on the LED if an 1 was received as first character
//    if ((char)payload[0] == '0') {
//      digitalWrite(RelayPin3, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    } else {
//      digitalWrite(RelayPin3, HIGH);  // Turn the LED off by making the voltage HIGH
//    }
//  }
//  else if (strstr(topic, sub_Fan4))
//  {
//    for (int i = 0; i < length; i++) {
//      Serial.print((char)payload[i]);
//    }
//    Serial.println();
//    // Switch on the LED if an 1 was received as first character
//    if ((char)payload[0] == '0') {
//      digitalWrite(DHTPin2, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    } else {
//      digitalWrite(DHTPin2, HIGH);  // Turn the LED off by making the voltage HIGH
//    }    
//  }
//  
//  else if ( strstr(topic, sub_servo))
//  {
//    for (int i = 0; i < length; i++) {
//      Serial.print((char)payload[i]);
//    }
//    Serial.println();
////    // Switch on the LED if an 1 was received as first character
////    if ((char)payload[0] == '0') {
////      digitalWrite(RelayPin4, LOW);   // Turn the LED on (Note that LOW is the voltage level
////    } else {
////      digitalWrite(RelayPin4, HIGH);  // Turn the LED off by making the voltage HIGH
////    }
//  }
//  
//  else
//  {
//    Serial.println("unsubscribed topic");
//  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  dht2.begin();
  pinMode(DHTPin1, INPUT);  
  pinMode(DHTPin2, INPUT);
//  pinMode(RelayPin1, OUTPUT);
//  pinMode(RelayPin2, OUTPUT);
//  pinMode(RelayPin3, OUTPUT);



  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF
//  digitalWrite(RelayPin1, HIGH);
//  digitalWrite(RelayPin2, HIGH);
//  digitalWrite(RelayPin3, HIGH);
//  digitalWrite(RelayPin4, HIGH);
  
  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);

  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}


void pubsen1(){
////////////READ DATA SENSOR 1
int h1 = dht.readHumidity();
delay(500);
// Read temperature as Celsius (the default)
int t1 = dht.readTemperature();
////////////READ DATA SENSOR 2
delay(500);
int h2 = dht2.readHumidity();
// Read temperature as Celsius (the default)
delay(500);
int t2 = dht2.readTemperature();
delay(500);
//send Data sensor
unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    //value berisi data yang di inisiasi
    DynamicJsonDocument doc(1024);
    temp1 = t1;
    delay(500);
    humadity1 = h1;
    delay(500);
    temp2 = t2;
    delay(500);
    humadity2 = h2;
    
    //Json fill data here
    doc["temp1"] = temp1;
    doc["humadity1"] = humadity1;
    delay(500);
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
    
    //Print serial data
    //snprintf (msg, MSG_BUFFER_SIZE, "{""temperature"" :%d ""humidity"" :%d}", temp1,humadity1);
    //snprintf (msg2, MSG_BUFFER_SIZE2, "%ld", humadity1);
    //Serial.print("Publish message1: ");
    //Serial.print(msg);
    //Serial.println(msg2);
    //output topic dan isi topic
    //client.publish("temp:", msg);
    //client.publish("humadity", msg2);
  }
Serial.println("");
Serial.print("Humidity1: ");
Serial.println(h1);
Serial.print("Temperature1: ");
Serial.println(t1);
Serial.print("Humidity2: ");
Serial.println(h2);
Serial.print("Temperature2: ");
Serial.println(t2);
//delay(2000);
  }

void pubsen2(){
float h = dht2.readHumidity();
// Read temperature as Celsius (the default)
float t = dht2.readTemperature();

//send Data sensor
unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    //value berisi data yang di inisiasi
    temp2 = t;
    humadity2 = h;
    snprintf (msg3, MSG_BUFFER_SIZE, "%ld", temp2);
    snprintf (msg4, MSG_BUFFER_SIZE2, "%ld", humadity2);
    Serial.print("Publish message2: ");
    Serial.print(msg3);
    Serial.println(msg4);
    //output topic dan isi topic
    client.publish("temp2", msg3);
    client.publish("humadity2", msg4);
    delay(5000);
  }
Serial.print("Humidity2: ");
Serial.println(h);
Serial.print("Temperature2: ");
Serial.println(t);
delay(1000);
  }


void loop() {
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  client.loop();
pubsen1();
//pubsen2();
}
