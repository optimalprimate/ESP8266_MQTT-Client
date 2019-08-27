#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 14 
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);
const char* ssid = "***";
const char* password =  "***";
const char* mqttServer = "192.168.1.***";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
int holding_var = 0;
int temp_pin = 0;
int j = 0;
int led = 4;

WiFiClient espClient;
PubSubClient client(espClient);

void setup(void) 
{ 
 Serial.begin(9600); 
 pinMode(led, OUTPUT); 
 sensors.begin(); 
WiFi.mode(WIFI_STA);
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
 if (client.connect("ESP8266Client1")) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("esp/test", "Hello from ESP1");
  client.subscribe("esp/restart");
  Serial.println("subscribing to esp/restart");
}
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
  //  Serial.print((char)payload[i]);
    holding_var = (payload[i]);
     Serial.println(holding_var);
      if(holding_var == 49) {
        Serial.println("ESP restart");
      ESP.restart();
    }
  }
}

 
void loop(void) 
{ 

 if (j > 200) { 
  Serial.println("reading sensors now: ");
  digitalWrite(led, HIGH);
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
   Serial.println("Publishing reading to MQTT");
    client.publish("esp/temp3", String(sensors.getTempCByIndex(0)).c_str());
 j=0;
 digitalWrite(led, LOW);
 }
client.loop();
j = j+1;
Serial.println("...");
delay(30);
   
} 
