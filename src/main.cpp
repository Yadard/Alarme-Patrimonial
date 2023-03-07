/*
  Modbus-Arduino Example - Test Holding Register (Modbus IP ESP8266)
  Configure Holding Register (offset 100) with initial value 0xABCD
  You can get or set this holding register
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Modbus.h>
#include <ModbusIP_ESP8266.h>

#define PORTA_PIN 5
#define PRESENCA_PIN 7
#define LDR_PIN A0
#define LED_PIN 6

// Modbus Registers Offsets (0-9999)
enum class REGS {
  SENSOR_PORTA,
  SENSOR_PRESENCA,
  SENSOR_LASER,
  ARMAR,
  DESARMAR,
  TESTE,
};
enum class State { DESARMADO, ARMADO, ALARME };

// ModbusIP object
ModbusIP mb;

void setup() {
  Serial.begin(115200);

  Serial.setDebugOutput(true);
  WiFi.mode(WIFI_AP_STA);
  mb.config("Alarme Patriomonial", "12345678");

  mb.addHreg(static_cast<word>(REGS::SENSOR_PORTA),
             static_cast<int>(State::DESARMADO));
  mb.addHreg(static_cast<word>(REGS::SENSOR_PRESENCA),
             static_cast<int>(State::DESARMADO));
  mb.addHreg(static_cast<word>(REGS::SENSOR_LASER),
             static_cast<int>(State::DESARMADO));
  mb.addHreg(static_cast<word>(REGS::ARMAR), 0);
  mb.addHreg(static_cast<word>(REGS::DESARMAR), 0);
  mb.addHreg(static_cast<word>(REGS::TESTE), 0);
}

void loop() {
  if (mb.Hreg(static_cast<word>(REGS::ARMAR))) {
    mb.Hreg(static_cast<word>(REGS::SENSOR_PORTA),
            static_cast<int>(State::ARMADO));
    mb.Hreg(static_cast<word>(REGS::SENSOR_PRESENCA),
            static_cast<int>(State::ARMADO));
    mb.Hreg(static_cast<word>(REGS::SENSOR_LASER),
            static_cast<int>(State::ARMADO));
  }

  if (mb.Hreg(static_cast<word>(REGS::DESARMAR))) {
    mb.Hreg(static_cast<word>(REGS::SENSOR_PORTA),
            static_cast<int>(State::DESARMADO));
    mb.Hreg(static_cast<word>(REGS::SENSOR_PRESENCA),
            static_cast<int>(State::DESARMADO));
    mb.Hreg(static_cast<word>(REGS::SENSOR_LASER),
            static_cast<int>(State::DESARMADO));
  }

  if (mb.Hreg(static_cast<word>(REGS::TESTE))) {
    mb.Hreg(static_cast<word>(REGS::SENSOR_PORTA),
            static_cast<int>(State::ALARME));
    mb.Hreg(static_cast<word>(REGS::SENSOR_PRESENCA),
            static_cast<int>(State::ALARME));
    mb.Hreg(static_cast<word>(REGS::SENSOR_LASER),
            static_cast<int>(State::ALARME));
  }

  mb.task();
}
