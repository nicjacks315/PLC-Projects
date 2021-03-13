# MAASyOS
Modular Automated Aquaponics System Operating System

Nicholas Jackson

SFSU Spring 2021 Senior Design Project

## High Level
- Implementations are defined in `aquaponics.hpp`
- Instantiations are declared in `config.hpp`
- Runtime commands are defined and registered in `commands.hpp`

## Features

### Control Systems

### Serialization

### Bluetooth Radio

### Circuit Diagram

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
#### RS-Latches
`Class Latch`
#### Metrics
`Class Metric`
#### Scheduled Tasks
`Class ScheduledTask`

### Sensor Types
#### Digital
#### Analog
#### Serial
#### PWM
