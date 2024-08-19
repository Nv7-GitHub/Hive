#include <LoRa.h>

const int PACKET_SIZE = 25;

void setup() {
  Serial.begin(115200);
  while (!Serial)
  Serial.println("Connected!");

  LoRa.setPins(10, 2, 3);
  LoRa.setSpreadingFactor(9);
  LoRa.setSignalBandwidth(31.25E3);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(8);
  LoRa.enableCrc();
  LoRa.onReceive(onReceive);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.receive(PACKET_SIZE);

  Serial.println("Ready to receive!");
}

void loop() {
  
}

struct Data {
  uint8_t id;

  float humidity;
  float temp;
  float pressure;

  float gps_lat;
  float gps_long;
  float gps_alt;

  uint32_t time;
};

void onReceive(int packetSize) {
  struct Data data;
  LoRa.readBytes((char*)(&data), PACKET_SIZE);

  Serial.print("size:");
  Serial.print(packetSize);
  Serial.print(",id:");
  Serial.print(data.id);
  Serial.print(",temp:");
  Serial.print(data.temp);
  Serial.print(",humidity:");
  Serial.print(data.humidity);
  Serial.print(",pressure:");
  Serial.print(data.humidity);
  Serial.print(",lat:");
  Serial.print(data.gps_lat);
  Serial.print(",long:");
  Serial.print(data.gps_long);
  Serial.print(",alt:");
  Serial.print(data.gps_alt);
}