#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <MQTT.h>
#include <time.h>
#include "secrets.h"
#define CORRENTE  4
#define TENSAO 2
BearSSL::WiFiClientSecure net;
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);


PubSubClient client(net);
time_t now;
time_t nowish = CURRENT_TIME_APROXIMATION_AS_EPOCH_SECONDS;
unsigned long last_mueasurement_publish_in_millis = 0;
void pubSubErr(int8_t MQTTErr){
  if (MQTTErr == MQTT_CONNECTION_TIMEOUT)
    Serial.print("Connection tiemout");
  else if (MQTTErr == MQTT_CONNECTION_LOST)
    Serial.print("Connection lost");
  else if (MQTTErr == MQTT_CONNECT_FAILED)
    Serial.print("Connect failed");
  else if (MQTTErr == MQTT_DISCONNECTED)
    Serial.print("Disconnected");
  else if (MQTTErr == MQTT_CONNECTED)
    Serial.print("Connected");
  else if (MQTTErr == MQTT_CONNECT_BAD_PROTOCOL)
    Serial.print("Connect bad protocol");
  else if (MQTTErr == MQTT_CONNECT_BAD_CLIENT_ID)
    Serial.print("Connect bad Client-ID");
  else if (MQTTErr == MQTT_CONNECT_UNAVAILABLE)
    Serial.print("Connect unavailable");
  else if (MQTTErr == MQTT_CONNECT_BAD_CREDENTIALS)
    Serial.print("Connect bad credentials");
  else if (MQTTErr == MQTT_CONNECT_UNAUTHORIZED)
    Serial.print("Connect unauthorized");
}

void connectToMqtt(bool nonBlocking = false)
{
  Serial.print("MQTT connecting ");
  while (!client.connected())
  {
    if (client.connect(DEVICE_ID, MQTT_USER, NULL))
    {
      Serial.println("connected!");
      if (!client.subscribe(MQTT_SUB_CLOUD_TO_DEVICE_TOPIC))
        pubSubErr(client.state());
      if (!client.subscribe(MQTT_SUB_INVOKE_TOPIC))
        pubSubErr(client.state());

      
    }
    else
    {
      Serial.print("SSL Error Code: ");
      Serial.println(net.getLastSSLError());
      Serial.print("failed, reason -> ");
      pubSubErr(client.state());
      if (!nonBlocking)
      {
        Serial.println(" < try again in 5 seconds");
        delay(5000);
      }
      else
      {
        Serial.println(" <");
      }
    }
    if (nonBlocking)
      break;
  }
}

void connectToWiFiVerbosely(String init_str){
  Serial.print(init_str);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("ok!");
}

void connectToWiFiSilently(){
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }
}

void NTPConnect(void){
  Serial.print("Setting up time using NTP");

  // Configure the NTP servers
  configTime(UTC_TIME_ZONE_BRAZIL_SECONDS, DST_SECONDS, NTP_SERVER_0, NTP_SERVER_1);
  
  // Wait for the current time to be updated
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(ctime(&now));
}

void checkWiFiThenMQTT(void){
  connectToWiFiVerbosely("Checking WiFi");
  connectToMqtt();
}

void mqtt_publish_with_topic(String topic, String payload=String("")){
  if (client.connected())
    if(client.publish(topic.c_str(), payload.c_str()))
      pubSubErr(client.state());
}

void mqtt_publish(String payload){
  if (client.connected())
    if (client.publish(MQTT_PUB_DEVICE_TO_CLOUD_TOPIC, payload.c_str()))
      pubSubErr(client.state());
}
void invoke_reply(String status_code, String rid, String payload=String("")){
  String topic = String(MQTT_PUB_INVOKE_REPLY_TOPIC);
  topic.replace("{status_code}", status_code.c_str());
  topic.replace("{rid}", rid.c_str());

  mqtt_publish_with_topic(topic, payload);
}
String perfume= String("perfume");


void mqtt_client_callback(char *topic, byte *payload, unsigned int length){
  String parsed_payload = String((char *) payload).substring(0, length);
  Serial.printf("Received [%s]: %s\r\n", topic, parsed_payload.c_str());
  String parsed_topic = String(topic);
  parsed_topic.replace(MQTT_SUB_INVOKE_BASE_TOPIC, "");
  uint8_t index_of_slash = parsed_topic.indexOf('/');
  uint8_t index_of_equals = parsed_topic.indexOf('=');
  String method = parsed_topic.substring(0, index_of_slash);
  String rid = parsed_topic.substring(index_of_equals + 1, -1);
  Serial.printf("This is the method: %s\r\n", method.c_str());
  Serial.printf("This is the rid: %s\r\n", rid.c_str());
  String status_code;
  String resp_payload;
  parsed_payload.replace('"','/');
  if (method == "ping"){
    status_code = "200";
  }
  else if (method == "ping_verbose"){
    status_code = "200";
    resp_payload = "{"
      "\"response_ping_key_0\": \"response_ping_value_0\","
      "\"response_ping_key_1\": \"response_ping_value_1\""
    "}";
  }

  else if (method =="change_parfum"){
        perfume=parsed_payload;
        status_code = "200";
        resp_payload = corRBG(parsed_payload);
  }
  else {
    status_code = "400";
    resp_payload = "{"
      "\"error\": \"command not supported by this device\""
    "}";
  }
  invoke_reply(status_code, rid, resp_payload);

}



void setup(){

  Serial.begin(115200);
  delay(5000);
  pinMode(TENSAO,OUTPUT);
  pinMode(CORRENTE,OUTPUT);
  

  Serial.println();
  Serial.println();
  WiFi.hostname(DEVICE_ID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  connectToWiFiVerbosely(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  NTPConnect();

  net.setInsecure();
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(mqtt_client_callback);

  connectToMqtt();

}

/**
 * @brief Loop routine of the device. 
 * This is called repeatedly after the setup routine. This repetition continues forever.
 * 
 */

void loop(){
  now = time(nullptr);
  if (!client.connected())
  {
    checkWiFiThenMQTT();
  }
  else
  {

    client.loop();
    if (millis() - last_mueasurement_publish_in_millis > 5000)
    {
        last_mueasurement_publish_in_millis = millis();
         
    }

  }
}