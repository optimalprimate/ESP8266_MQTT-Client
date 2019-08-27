#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
const char* ssid = "***";
const char* password =  "***";
const char* mqttServer = "192.168.1.***";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
int holding_var = 0;
 
WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
    // Transmitter is connected to ESP LOLin pin D2 which is GPIO4
mySwitch.enableTransmit(4); 
mySwitch.setPulseLength(318);
mySwitch.setRepeatTransmit(5);
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
 //   if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 if (client.connect("ESP8266Client")) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
 
}



void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
  //  Serial.print((char)payload[i]);
    holding_var = (payload[i]);
     Serial.println(holding_var);
      if(holding_var == 49) {
      mySwitch.sendTriState("0F000FF0FF0F");
      Serial.println("RF ON");
    }
     if(holding_var == 48) {
      mySwitch.sendTriState("0F000FF0FFF0");
      Serial.println("RF OFF");
    }
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();
}
