/*
  Modbus-Arduino Example - Test Holding Register (Modbus IP ESP8266)
  Configure Holding Register (offset 100) with initial value 0xABCD
  You can get or set this holding register
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <ESP8266WiFi.h>
#include <Modbus.h>
#include <ModbusIP_ESP8266.h>

// Modbus Registers Offsets (0-9999)
const int TEST_HREG = 100;


//ModbusIP object
ModbusIP mb;
  
void setup() {
  Serial.begin(115200);
 
  Serial.setDebugOutput(true);
  mb.config("Alarme Patriomonial", "12345678");


  mb.addHreg(TEST_HREG, 0xABCD);
}
 
void loop() {
   //Call once inside loop() - all magic here
   mb.task();
}
