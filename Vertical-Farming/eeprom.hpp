#ifndef EEPROM_HPP
#define EEPROM_HPP

#include <EEPROM.h>

* EEPROM Write

 *

 * Stores values read from analog input 0 into the EEPROM.

 * These values will stay in the EEPROM when the board is

 * turned off and may be retrieved later by another sketch.

 */

#include <EEPROM.h>

//humidity;//OR to store a history of the value, use Metric<float> airHumidity; SUBJECT TO MEMORY LIMITS
//airTemp;
//waterContact;
//ppm;
//acidity;
//flowRate;
//waterTemp;

int addr = 0;

void setup() {

}

void loop() {

  int val = analogRead(0) / 4;

  EEPROM.write(addr, val);

  addr = addr + 1;

  if (addr == EEPROM.length()) {
    addr = 0;
  }

}

#endif
