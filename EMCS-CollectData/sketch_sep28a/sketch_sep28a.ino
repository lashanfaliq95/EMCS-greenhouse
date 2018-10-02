#include <PubSubClient.h>
#include <ESP8266WiFi.h>
//#include <BH1750.h>
#include <Wire.h>
#include <string.h>


#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 0//pin D3 for dht11
#define dht_dpin2 5//pin D1 for dht11 external
#define MQTT_SERVER "192.168.8.104"


void readSensors();
void callback(char* topic, byte* payload, unsigned int length);
String macToStr(const uint8_t* mac);

DHT dht(dht_dpin, DHTTYPE);
DHT dht2(dht_dpin2, DHTTYPE);
//BH1750 lightMeter;
WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);
String clientName;
const char* ssid = "D4G";
const char* password = "palayanbn";




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("hello");
  pinMode(D5  , OUTPUT);
  pinMode(D6  , OUTPUT);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  //intialising the dht11 sensors
  dht.begin();
  dht2.begin();

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
  
  float h;
  float t;
do{
  delay(100);
  h = dht.readHumidity();
  t = dht.readTemperature();
}while(h != h);



int acuatorMatrix[4][2] = { {1, 1},  {1, 0},{0, 1},{0, 0}};

 int actuator_control[2];
 int temp=random(0,3);
  actuator_control[0]= acuatorMatrix[temp][0];
  actuator_control[1]= acuatorMatrix[temp][1];
  int rand_;
  int wait_time;
  String ran;
  if(actuator_control[0] == 0){
    digitalWrite(D5, HIGH); //fan
  } else {
    digitalWrite(D5, LOW);
  }
  if(actuator_control[1] == 0){
    digitalWrite(D6, HIGH);//humidifier
    rand_ = random(1,10);
    wait_time = rand_ * 1000 * 60;
    ran = String(rand_ * 60);
  } else {
    digitalWrite(D6, LOW);
    rand_ = random(10,30);
    ran = String(rand_);
    wait_time = rand_ * 1000;
  }
  String fanControl=String(actuator_control[0]);
  String humidifierControl=String(actuator_control[1]);
  String tempVal=String(t); 
  String humVal=String(h);
  
  
  String json="{\"fan\":"+fanControl+", \"humidifier\":"+humidifierControl+", \"duration\":"+ran  +", \"start_temp\":"+tempVal+", \"end_temp\": 0, \"start_hum\":"+humVal+", \"end_hum\":0}";
   //String json="{\"fan\":"+fanControl+", \"humidifier\":"+humidifierControl+", \"duration\":"+ran  +", \"start_temp\":"+tempVal+", \"start_temp_external\":"+tempVal_ex+", \"end_temp\": 0, \"start_hum\":"+humVal+", \"start_hum_external\":"+humVal_ex+", \"end_hum\":0}";
  char *data = "";
  json.toCharArray(data, json.length()+1);
  
  client.publish("newData", data);

  // todo: actuator control code here
  Serial.println("waiting for " + wait_time);
  delay(wait_time);
  if (!client.loop()) {
    client.connect((char*) clientName.c_str());
  } 
  float h2 = dht.readHumidity();
  float t2 = dht.readTemperature();
  float h_ex;
 float t_ex;

  h_ex = dht2.readHumidity();
  t_ex = dht2.readTemperature();

  String tempVal2=String(t2); 
  String humVal2=String(h2);
  String tempVal_ex=String(t_ex); 
  String humVal_ex=String(h_ex);
  String json2 = "{\"end_temp\": "+tempVal2+",\"temp_external\":"+tempVal_ex+", \"end_hum\":"+humVal2+", \"hum_external\":"+humVal_ex+"}";
  char *data2 = "";
  json2.toCharArray(data2, json2.length()+1);
  client.publish("updateData", data2);
  Serial.println("waiting for " + wait_time);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  delay(60000);
  free(data);
  free(data2);
  //client.loop();
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
