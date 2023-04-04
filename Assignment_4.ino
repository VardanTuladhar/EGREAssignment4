#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
// Replace with your WiFi credentials
const char* ssid = "8.5 Canal";
//const char* password = "Hoohvt18!";

// Replace with your MQTT broker IP address or hostname
const char* mqtt_server = "test.mosquitto.org";
const int led = LED_BUILTIN;
int angle;
int pos;

Servo myservo;

// Initialize the WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);
void callback1(char* topic, byte* payload, unsigned int length) {
  // Print out the received message
  Serial.print("Message received on topic [");
  Serial.print("vardan/light");
  Serial.print("]: ");
  String messageInfo;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageInfo += (char)payload[i];
  }

  Serial.println();
  Serial.println(messageInfo);
}
void callback(char* topic, byte* payload, unsigned int length) {
  // Print out the received message
  
  String messageInfo;
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    messageInfo += (char)payload[i];
  }
  if(messageInfo == "on" || messageInfo == "off")
  {
    Serial.print("Message received on topic [");
    Serial.print("vardan/light");
    Serial.print("]: ");
    Serial.println(messageInfo);
    if(messageInfo == "on")
    {
      digitalWrite(led, LOW);  
    }
    else if(messageInfo == "off")
    {
      digitalWrite(led, HIGH);
    }
  }
  else
  {
    Serial.print("Message received on topic [");
    Serial.print("vardan/swing");
    Serial.print("]: ");    
    Serial.println(messageInfo);
    angle = messageInfo.toInt();
  }

  


}
void setup() {
  // Connect to WiFi
  Serial.begin(9600);
  WiFi.begin(ssid, NULL);
  pinMode(led, OUTPUT);
  myservo.attach(D2);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  // Connect to MQTT broker
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.println("Connecting to MQTT broker...");
  while (!client.connected()) {
    if (client.connect("WemosClient")) {
      Serial.println("MQTT broker connected");
      client.subscribe("vardan/light");
      client.subscribe("vardan/swing");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      Serial.println("Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  
  // Check for incoming messages
  client.loop();
  for (pos = 0; pos <= angle; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(1);           // waits 15ms for the servo to reach the position
  }
  for (pos = angle; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(1);                           // waits 15ms for the servo to reach the position
  }
}

