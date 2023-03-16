/*
  Modbus-Arduino Example - Test Holding Register (Modbus IP ESP8266)
  Configure Holding Register (offset 100) with initial value 0xABCD
  You can get or set this holding register
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>

#define PORTA_PIN 7
#define PRESENCA_PIN A0
#define LDR_PIN A3
#define LED_PIN 3
#define BUZZER_PIN 4

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
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 133);
ModbusIP mb;

void setup() {
  Serial.begin(BAUDRATE);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PORTA_PIN, INPUT_PULLUP);


  mb.config(mac, ip);

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


  if(mb.Hreg(static_cast<word>(REGS::SENSOR_PORTA)) == static_cast<int>(State::ARMADO)){
    if(digitalRead(PORTA_PIN)){
      mb.Hreg(static_cast<word>(REGS::SENSOR_PORTA), static_cast<int>(State::ALARME));
    }
  }

  if(mb.Hreg(static_cast<word>(REGS::SENSOR_PRESENCA)) == static_cast<int>(State::ARMADO)){
    if(digitalRead(PRESENCA_PIN)){
      mb.Hreg(static_cast<word>(REGS::SENSOR_PRESENCA), static_cast<int>(State::ALARME));
    }
  }

  if(mb.Hreg(static_cast<word>(REGS::SENSOR_LASER)) == static_cast<int>(State::ARMADO)){
    digitalWrite(LED_PIN, HIGH);
    uint16_t ldr = analogRead(LDR_PIN);
    if(ldr < 150){
      mb.Hreg(static_cast<word>(REGS::SENSOR_LASER), static_cast<int>(State::ALARME));
      
    }
  } else if (mb.Hreg(static_cast<word>(REGS::SENSOR_LASER)) == static_cast<int>(State::DESARMADO)){
    digitalWrite(LED_PIN, LOW);
  }


  static uint32_t last_time = 0;
  uint32_t time = millis();
  if(mb.Hreg(static_cast<word>(REGS::SENSOR_LASER)) == static_cast<int>(State::ALARME) ||
     mb.Hreg(static_cast<word>(REGS::SENSOR_PORTA)) == static_cast<int>(State::ALARME) ||
     mb.Hreg(static_cast<word>(REGS::SENSOR_PRESENCA)) == static_cast<int>(State::ALARME)){
      digitalWrite(BUZZER_PIN, HIGH);
     } else {
      digitalWrite(BUZZER_PIN, LOW);
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
