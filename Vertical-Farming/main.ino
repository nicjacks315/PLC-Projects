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
}

void loop() {
  dt = clock.tick();
  serial.update();
  sensors.update();
  latches.check();
}
