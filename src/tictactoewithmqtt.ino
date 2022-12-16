#include <Keypad.h>
#include <WiFi.h>
#include <PubSubClient.h>
// define the symbols on the buttons of the keypad
char keys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[4] = { 14, 27, 26, 25 };  // connect to the row pinouts of the keypad
byte colPins[4] = { 13, 21, 22, 23 };
Keypad input = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);
int latchPin = 2;  // Pin connected to ST_CP of 74HC595（Pin12）
int clockPin = 4;  // Pin connected to SH_CP of 74HC595（Pin11）
int dataPin = 15;  // Pin connected to DS of 74HC595（Pin14）
// Define the pattern data for a smiling face
int smilingFace[] = {  //"^ⅴ^"
  0x24, 0x24, 0x7F, 0x24, 0x24, 0x7F, 0x24, 0x24
};

const char *ssid = "";    // Enter your WiFi name
const char *password = "";  // Enter WiFi password
// MQTT Broker
const char *mqtt_broker = "";  // Enter your WiFi or Ethernet IP
const char *topic = "test/topic";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  // set pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Set software serial baud to 115200;
  Serial.begin(115200);

  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());

    Serial.printf("The client %s connects to mosquitto mqtt broker\n", client_id.c_str());

    if (client.connect(client_id.c_str())) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  // publish and subscribe
  client.publish(topic, "ESP32 would like to play tic-tac-toe");
  client.subscribe(topic);
}
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println(" - - - - - - - - - - - -");
}

void loop() {
  // Define a one-byte variable (8 bits) which is used to represent the selected state of 8 column.
  client.loop();
  int cols;
  char play;
  // Display the static smiling pattern
  cols = 0x01;
  for (int i = 0; i < 8; i++) {     // display 8 column data by scaning
    matrixRowsVal(smilingFace[i]);  // display the data in this column
    matrixColsVal(~cols);           // select this column                   // display them for a period of time         // clear the data of this column
    delay(1);
    matrixRowsVal(0x00);
    cols <<= 1;  // shift"cols" 1 bit left to select the next column
  }
  play = input.getKey();
  if (play == '1') {
    smilingFace[0] = smilingFace[0] + 0xC0;
    client.publish(topic, "ESP placed O at position1");
    client.subscribe(topic);
  }
  if (play == '2') {
    smilingFace[3] = smilingFace[3] + 0xC0;
    client.publish(topic, "ESP placed O at position2");
    client.subscribe(topic);
  }
  if (play == '3') {
    smilingFace[6] = smilingFace[6] + 0xC0;
    client.publish(topic, "ESP placed O at position3");
    client.subscribe(topic);
  }
  if (play == '4') {
    smilingFace[0] = smilingFace[0] + 0x18;
    client.publish(topic, "ESP placed O at position4");
    client.subscribe(topic);
  }
  if (play == '5') {
    smilingFace[3] = smilingFace[3] + 0x18;
    client.publish(topic, "ESP placed O at position5");
    client.subscribe(topic);
  }
  if (play == '6') {
    smilingFace[6] = smilingFace[6] + 0x18;
    client.publish(topic, "ESP placed O at position6");
    client.subscribe(topic);
  }
  if (play == '7') {
    smilingFace[0] = smilingFace[0] + 0x03;
    client.publish(topic, "ESP placed O at position7");
    client.subscribe(topic);
  }
  if (play == '8') {
    smilingFace[3] = smilingFace[3] + 0x03;
    client.publish(topic, "ESP placed O at position8");
    client.subscribe(topic);
  }
  if (play == '9') {
    smilingFace[6] = smilingFace[6] + 0x03;
    client.publish(topic, "ESP placed O at position9");
    client.subscribe(topic);
  }
  if (play == 'A') {
    smilingFace[0] = 0x24;
    smilingFace[3] = 0x24;
    smilingFace[6] = 0x24;
  }
}
void matrixRowsVal(int value) {
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, LSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}
void matrixColsVal(int value) {
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}
