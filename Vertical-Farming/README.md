# MAASyOS
Modular Automated Aquaponics System Operating System

Nicholas Jackson

SFSU Spring 2021

Mechanical Engineering Senior Design Project

## Description
Manages a set of sensors and controls a series of 5V solid state relays which power high current devices such as air conditioners, fans, pumps, lighting systems, etc. Arduino UNO R3 is likely a less-than-optimal choice for this project due to limited dynamic memory, limited processing power, large form factor, and 8-10 bit A2D converter. A preferrable alternative would be something like the 32-bit Teensy 3.2, but we work with what we have. https://www.pjrc.com/teensy/

## High Level
- Implementations are defined in `aquaponics.hpp`
- Instantiations are declared in `config.hpp`
- Runtime commands are defined and registered in `commands.hpp`

## Features

### Control Systems
- to-do

### Serialization
- to-do

### Bluetooth Radio
DSD Tech HM-10 presoldered on breakout board.
Extensive testing on HM-10s, clones, and counterfeits performed by Martyn Currey at http://www.martyncurrey.com/hm-10-bluetooth-4ble-modules/.

### Circuit Diagram
- to-do

### Commands

| Command | Description | Syntax |
| :--- | --- | --- |
| `info` | Print a table of all sensor readings and relay states | `info` |
| `time` | Set the internal clock, defaults to compile time on each boot. Use 24-hour time to distinguish AM-PM. | `time <hour> <minute>` |
| `vars` | List the IDs of all changeable cvars | `vars` |
| `get` | Get the value of a cvar | `get <cvar ID>` |
| `set` | Set the value of a cvar | `set <cvar ID> <float>` |

### Components
#### Sensors
`Class Sensor`
- to-do
#### RS-Latches
`Class Latch`
- to-do
#### Metrics
`Class Metric`
- to-do
#### Scheduled Tasks
`Class ScheduledTask`
- to-do

### Sensor Types
#### Digital
- to-do
#### Analog
- to-do
#### Serial
- to-do
#### PWM
- to-do
