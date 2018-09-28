#include <PubSubClient.h>
#include <ESP8266WiFi.h>
//#include <BH1750.h>
#include <Wire.h>


#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 0//pin D3 for dht11
#define MQTT_SERVER "192.168.1.100"

void callback(char* topic, byte* payload, unsigned int length);
//
DHT dht(dht_dpin, DHTTYPE);
//BH1750 lightMeter;
WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);

const char* ssid = "Neal";
const char* password = "recursiveinternet";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("hello");

 WiFi.begin(ssid, password);
  if(WiFi.status() != WL_CONNECTED){
    //debug printing
    Serial.print("Connecting to ");
    Serial.println(ssid);

    //loop while we wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    //print out some more debug once connected
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);

      //if connected, subscribe to the topic(s) we want to be notified about
      if (client.connect((char*) clientName.c_str())) {
        Serial.print("\tMTQQ Connected");
      }
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  int rand_ = random(1, 5);
  int wait_time = rand_ * 1000 * 60;
  // todo:coontrol acutators and Read here sensor data and concat to relevent values

  
  client.publish("newData", 
  "{\"fan\":1, \"humidifier\":1, \"duration\":"+ String(rand_) +", \"start_temp\":25, \"end_temp\": 0, \"start_hum\":50, \"end_hum\":0}");
  
  Serial.println("waiting for " + wait_time);
  delay(wait_time);
  // todo: Read here sensor data and concat to relevent values
  client.publish("updateData", "{\"end_temp\": 20, \"end_hum\":60}");

}



void callback(char* topic, byte* payload, unsigned int length) {

  //convert topic to string to make it easier to work with
  String topicStr = topic; 

  //Print out some debugging info
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  
}

//generate unique name from MAC addr
String macToStr(const uint8_t* mac){

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);

    if (i < 5){
      result += ':';
    }
  }

  return result;
} 
