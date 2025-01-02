#include <LoRa.h>

const int PACKET_SIZE = 60;
const long frequency = 868500000; // 915.6 MHz

void setup() {
  Serial.begin(115200);
  while (!Serial);

  delay(100);
  Serial.println("Connected!");

  LoRa.setPins(10, 2, 3);
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(500E3);
  LoRa.setCodingRate4(5);
  LoRa.setPreambleLength(8);
  LoRa.enableCrc();

  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.receive();

  Serial.println("Ready to receive!");
}

void loop() {
  onReceive(LoRa.parsePacket());
}

#pragma pack(1)
struct Data {
  uint8_t id;

  float humidity;
  float temp;
  float pressure;

  float gps_lat;
  float gps_long;
  float gps_alt;

  uint32_t time;

  char satcnt;
};

// ChatGPT
void hexStringToStruct(const char *hexString, void *outputStruct, size_t size) {
    unsigned char *bytePtr = (unsigned char *)outputStruct;

    for (size_t i = 0; i < size; ++i) {
        // Read two hex characters and convert to a byte
        sscanf(hexString + (i * 2), "%2hhx", &bytePtr[i]);
    }
}


void onReceive(int packetSize) {
  if (packetSize == 0) {
    return;
  }
  /*Serial.print("GOT PACKET (size: ");
  Serial.print(packetSize);
  Serial.println(")");*/
  if (packetSize != PACKET_SIZE) {
    char data2[128];
    memset(&data2, 0, 128);
    LoRa.readBytes((char*)(&data2), packetSize);
    Serial.println(data2);
    return;
  }


  char dataRaw[PACKET_SIZE + 1];
  memset(&dataRaw[0], 0, PACKET_SIZE+1);
  LoRa.readBytes(&dataRaw[0], PACKET_SIZE);
  //Serial.println(&dataRaw[0]);

  struct Data data;
  hexStringToStruct(&dataRaw[0], &data, sizeof(data));

  Serial.print("size:");
  Serial.print(packetSize);
  Serial.print(",rssi:");
  Serial.print(LoRa.packetRssi());
  Serial.print(",id:");
  Serial.print(data.id);
  Serial.print(",temp:");
  Serial.print(data.temp);
  Serial.print(",humidity:");
  Serial.print(data.humidity);
  Serial.print(",pressure:");
  Serial.print(data.pressure);
  Serial.print(",lat:");
  Serial.print(data.gps_lat, 6);
  Serial.print(",long:");
  Serial.print(data.gps_long, 6);
  Serial.print(",satcnt:");
  Serial.print((int)data.satcnt);
  Serial.print(",alt:");
  Serial.println(data.gps_alt, 3);
}