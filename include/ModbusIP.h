/*
    ModbusIP_ESP8266.h - Header for Modbus IP ESP8266 Library
    Copyright (C) 2015 Andr� Sarmento Barbosa
*/
#include <Ethernet.h>
#include <Modbus.h>
#include <SPI.h>

#ifndef MODBUSIP_ESP8266_H
#define MODBUSIP_ESP8266_H

#define MODBUSIP_PORT 502
#define MODBUSIP_MAXFRAME 200
#define MODBUSIP_TIMEOUT 10

class ModbusIP : public Modbus {
private:
  byte _MBAP[7];

public:
  ModbusIP(byte *mac, IPAddress ip) : mac(mac), ip(ip) {}
  void setup();
  void task();

  byte *mac;
  IPAddress ip;
};

#endif // MODBUSIP_ESP8266_H
