#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
SoftwareSerial pmsSerial(14, 3);

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
  Serial.begin(115200);
 pinMode(led, OUTPUT); 

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
 if (client.connect("ESP8266Client3")) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("esp/test", "Hello from ESP3");
  client.subscribe("esp/restart");
  Serial.println("subscribing to esp/restart");

    pmsSerial.begin(9600);
} //end of setup
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

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
 
struct pms5003data data;
 
void loop(void) 
{ 
//wifi check
if (WiFi.status() != WL_CONNECTED){
  Serial.println("WIFI disconnect detected");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(5000);
  client.publish("esp/test","Wifi reconnect on esp1");
  Serial.println("Wifi recconected");
}

//MQTT check
if (!client.connected()) {
   client.setServer(mqttServer, mqttPort);
   delay(5000);
   Serial.println("Connecting to MQTT...");
   client.publish("esp/test","MQTT reconnect on esp1");
}

 if (j > 600) { 
    if (readPMSdata(&pmsSerial)) {
      Serial.print(data.pm10_standard);  Serial.print(","); 
      Serial.print(data.pm25_standard);  Serial.print(","); 
      Serial.print(data.pm100_standard); Serial.print(","); 
      client.publish("esp/pm1", String(data.pm25_standard).c_str());
      ESP.restart();
      }
   
 j=0;
 }
client.loop();
j = j+1;
Serial.println(j);
delay(1000);
} //end of loop
 
//pm2.5 sensor functions:
boolean readPMSdata(Stream *s) {
  if (! s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }
 // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}
