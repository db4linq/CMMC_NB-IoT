#define DEBUG

#include <Arduino.h>
#include <CMMC_NB_IoT.h> 
#include <AltSoftSerial.h>

CMMC_NB_IoT nb(&Serial2);

void setup()
{
  Serial.begin(57600);
  Serial2.begin(9600);

  Serial.setTimeout(4); 
  Serial2.setTimeout(4); 
  delay(10);
  Serial.println();
  Serial.println(F("Starting Application..."));
  Serial.println(F("Waiting Modem..."));

  nb.setDebugStream(&Serial); 
  nb.onDeviceReboot([]() {
    Serial.println(F("[user] Device rebooted."));
  });

  nb.onDeviceReady([](CMMC_NB_IoT::DeviceInfo device) {
    Serial.println("[user] Device Ready!");
    Serial.print(F("# Module IMEI-->  "));
    Serial.println(device.imei);
    Serial.print(F("# Firmware ver-->  "));
    Serial.println(device.firmware);
    Serial.print(F("# IMSI SIM-->  "));
    Serial.println(device.imsi);
    Serial.println("Activating NB-IoT shield.");
    nb.activate();
  });

  nb.onConnecting([]() {
	  Serial.println("Connecting to NB-IoT...");
    delay(500);
  });

  nb.onConnected([]() {
    Serial.println("[user] NB-IoT Network connected");
    char tmp[400];
    nb._writeCommand(F("AT+CSQ"), 10L, tmp);  // imsi sim
    String t = String(tmp);
    t.replace("OK", " - OK");
    Serial.println(t);
    int ct = 1;
    int sockId = nb.createUdpSocket("159.89.205.216", 11221, UDPConfig::DISABLE_RECV);
    nb.createUdpSocket("159.89.205.216", 11222, UDPConfig::DISABLE_RECV);
    String _tmp = "";
    // while(1) {
    //   _tmp += String(ct) + "-";
    //   Serial.println(String("payload size = ") + _tmp.length()) + String("byte");
    //   nb.sendMessage(_tmp); 
    //   delay(2000);
    //   ct++;
    // }
  });

  nb.begin();
}

void loop()
{ 
	nb.loop();
}