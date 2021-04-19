#include "utils.hpp"
#include "clock.hpp"
#include "pins.hpp"
#include "console.hpp"
#include "serial.hpp"
#include "cvars.hpp"
#include "metric.hpp"
#include "display.hpp"
#include "sensor.hpp"
#include "latch.hpp"
#include "aquaponics.hpp"
#include "config.hpp"
#include "commands.hpp"

void setup() {
  configure();
  clock.initialize();
  serial.initialize();
  console.initialize();
  sensors.initialize();
  latches.initialize();
  
  pinMode( PIN_OUT_LIT, OUTPUT );
  
}

unsigned long timer = 0UL;

void loop() {
  dt = clock.tick();
  
  //temporary solution for light control
  digitalWrite( PIN_OUT_LIT, clock.getHours() >= 8 && clock.getHours() <= 20 ? HIGH : LOW );
  
  serial.update();
  sensors.update();
  latches.check();
}
