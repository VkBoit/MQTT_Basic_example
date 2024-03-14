#include <Arduino.h>
#include <WiFi.h>
#include "PubSubClient.h"

// Wifi credentials
const char *ssid = "VENO";
const char *password = "vahs2023";

// MQTT Broker details
const char *mqtt_broker = "broker.emqx.io";      // EMQX broker
const char *topic = "emqx/esp32";
const char *mqtt_username = "emqx";               // EMQX
const char *mqtt_password = "public";         // EMQX
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// put function declarations here:
// put function definitions here:
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("------------------------------------------");
}

void setup() {
  Serial.begin(115200);

  // Connect to Wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi.");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(" .");
  }
  Serial.println();
  Serial.println("Connected!");

  //connect to MQTT broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while(!client.connected()) {
    String client_id = "ESP-32-Kenya";
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public Baridi MQTT broker connected");
    }
    else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  // Publish and subscribe
  client.publish(topic, "Hello from Kenya");
  client.subscribe(topic);
}

void loop() {
  client.loop();
}
