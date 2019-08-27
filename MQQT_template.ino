#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "***";
const char* password =  "***";
const char* mqttServer = "192.168.19***";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
int j = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup(void) 
{ 
 Serial.begin(9600); 

WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);

 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
 //   if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 if (client.connect("ESP8266Client3")) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("esp/test", "Hello from ESP8266");
}

 
void loop(void) 
{ 
// if (j > 800000) { 
 client.publish("esp/pm1", String("heartbeat").c_str());
// j=0;
// }  

 delay(300);
//j = j+1;
   
} 
