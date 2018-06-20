#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <BH1750.h>
#include <Wire.h>


#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 0//pin D3 for dht11
#define MQTT_SERVER "192.168.8.100"

void readSensors();
void callback(char* topic, byte* payload, unsigned int length);

DHT dht(dht_dpin, DHTTYPE);
BH1750 lightMeter;
WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);

const char* ssid = "D4G";
const char* password = "NoInternet";
const int lightPin = D4;

char* lightTopic = "light";
char* tempTopic = "temperature";
char* humidTopic = "humidity";
char* soilMoistureTopic = "soilMoisture";
char * fan="fan";
char * pump="pump";

int sense_Pin = 0; // sensor input at Analog pin A0 for the soil moisture sensor
int value = 0;



void setup() {
  //initialize the light as an output and set to LOW (off)
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
 // digitalWrite(lightPin, HIGH);

  //intialising the dht11 sensors
  dht.begin();
  
  //start the serial line for debugging
  Serial.begin(115200);
  delay(500);


  //start wifi subsystem
  WiFi.begin(ssid, password);
  //attempt to connect to the WIFI network and then connect to the MQTT server
  reconnect();

  //wait a bit before starting the main loop
   delay(2000);
}



void loop(){

  //reconnect if connection is lost
  if (!client.connected() && WiFi.status() == 3) {reconnect();}

  //maintain MQTT connection
  client.loop();
 
  //MUST delay to allow ESP8266 WIFI functions to run
  delay(100); 
         
   readSensors();

}


void callback(char* topic, byte* payload, unsigned int length) {

  //convert topic to string to make it easier to work with
  String topicStr = topic; 

  //Print out some debugging info
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  Serial.println(topicStr);
   
  if(topicStr.equals("fan")){
  //turn the light on if the payload is '1' and publish to the MQTT server a confirmation message
  if(payload[0] == '1'){
     digitalWrite(D4, HIGH);
     delay(100);
//   client.publish("testTopic", "Light On");
//   
Serial.println("on");

  }

  //turn the light off if the payload is '0' and publish to the MQTT server a confirmation message
  else if (payload[0] == '0'){
   digitalWrite(D4, LOW);
   delay(100);
//   client.publish("testTopic", "Light Off");
//   
Serial.println("off");
  }
  }

    if(topicStr.equals("pump")){
  //turn the light on if the payload is '1' and publish to the MQTT server a confirmation message
  if(payload[0] == '1'){
     digitalWrite(D5, HIGH);
     delay(100);
//   client.publish("testTopic", "Light On");
//   
Serial.println("on");

  }

  //turn the light off if the payload is '0' and publish to the MQTT server a confirmation message
  else if (payload[0] == '0'){
   digitalWrite(D5, LOW);
   delay(100);
//   client.publish("testTopic", "Light Off");
//   
Serial.println("off");
  }
  }

      if(topicStr.equals("humidifier")){
  //turn the light on if the payload is '1' and publish to the MQTT server a confirmation message
  if(payload[0] == '1'){
     digitalWrite(D6, HIGH);
     delay(100);
//   client.publish("testTopic", "Light On");
//   
Serial.println("on");

  }

  //turn the light off if the payload is '0' and publish to the MQTT server a confirmation message
  else if (payload[0] == '0'){
   digitalWrite(D6, LOW);
   delay(100);
//   client.publish("testTopic", "Light Off");
//   
Serial.println("off");
  }
  }
  
  

  

}




void reconnect() {
  
//attempt to connect to the wifi if connection is lost
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

  }

  //make sure we are connected to WIFI before attemping to reconnect to MQTT
  if(WiFi.status() == WL_CONNECTED){
  // Loop until we're reconnected to the MQTT server
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");

      // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);

      //if connected, subscribe to the topic(s) we want to be notified about
      if (client.connect((char*) clientName.c_str())) {
        Serial.print("\tMTQQ Connected");
        client.subscribe(fan);
         client.subscribe(pump);

      }

      //otherwise print failed for debugging
      else{Serial.println("\tFailed."); abort();}
    }
  }
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

void readSensors() {
  //humidity and temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();

//soil moisture level
  value = analogRead(sense_Pin);
  value = value / 10;
  
//light intensity
  double light = 10;

char  tempt[10];
char temph[10];
char tempvalue[10];
  
 dtostrf(t,4,4,tempt);
 dtostrf(h,4,4,temph);
 dtostrf(value,4,4,tempvalue);
 
  client.publish(lightTopic, "Light On");
  client.publish(tempTopic, tempt);
  client.publish(humidTopic,temph);
  client.publish(soilMoistureTopic,tempvalue);
}

