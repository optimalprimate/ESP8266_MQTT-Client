#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "***";
const char* password =  "***";
const char* mqttServer = "192.168.19***";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
//millis for delayed looping
int delay_period = 5000; //every 5seconds
unsigned long time_now = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
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

//reconnect loop for MQTT Dropout
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void loop() { 
  if (!client.connected()) {
    reconnect();
  }

 if(millis() > time_now + delay_period){
        time_now = millis();
        client.publish("esp/topic", String("value").c_str());
    }
} 
