# MAASyOS
Modular Automated Aquaponics System Operating System

Nicholas Jackson, EIT

SFSU Department of Mechanical Engineering, Spring 2021

![serial-ble-terminal](https://user-images.githubusercontent.com/80596121/111042939-992c2c80-83f4-11eb-8c24-d73a0afcbe69.jpg)

## Description
https://www.theaquaponicsource.com/what-is-aquaponics/

Manages a set of sensors and controls a series of 5V solid state relays which power high current devices such as air conditioners, fans, pumps, lighting systems, etc. Arduino UNO R3 ATMega328p is a less-than-optimal choice for this project due to limited dynamic memory, limited processing power, large form factor, and 8-10 bit A2D converter. A preferrable alternative would be something like the 32-bit Teensy 3.2, but we work with what we have. https://www.pjrc.com/teensy/

## Code Organization
- Implementations are defined in `aquaponics.hpp`
- Instantiations are declared in `config.hpp`
- Runtime commands are defined and registered in `commands.hpp`

## Control Systems
Setpoints are defined in `config.hpp` and are dependent on fish and crop species population ratios. These may all be adjusted via commands documented below.

![Set-point_control](https://user-images.githubusercontent.com/80596121/111044026-f0cd9680-83fa-11eb-96c2-f8cf8ae445cd.png)
| System | Signal | Measuring Element | Effector |
| --- | --- | --- | --- |
| Relative Humidity | % | DHT22 | Can Fan |
| Air Temperature | °C | DHT22 | 1500 BTU Air Conditioner |
| Water Temperature | °C | pH Probe | 1kW Water Heater |
| Water Acidity | pH | pH Probe | pH Dosing Mechanism |
| Water Total Dissolved Solids | ppm | TDS Probe | Human Operator |
| Water Flow Rate | Lpm | 1/2" Φ Flowmeter | Water Pump |
| Water Level | Boolean | Infrared Water Contact Sensor | Water Pump |
| Daylight | Elapsed Time | Onboard Timer | Human Operator |

## Serialization
A lack of supplementary serial pins (TX/RX pins are pulled up directly from the USB-A port) on the UNO R3 means that functionality needs to be mimicked with software on digital pins that support interrupts. The following libraries are used interchangably through different versions, but ultimately `SoftwareSerial` was chosen.
- `SoftwareSerial` https://www.arduino.cc/en/Reference/softwareSerial
  - Interrupt heavy, but reliable. Causes issues with timing if used too much.
- `AltSoftSerial` https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
  - Goes a lot easier on the pin interrupts, but also yields strange behavior if used creatively.
- `NewSoftSerial` http://arduiniana.org/libraries/newsoftserial/
  - Not a lot of testing went into this one.

Note that all serial monitoring should use CR & LF. These characters are automatically stripped prior to forwarding to the BLE radio.

## Bluetooth Radio
DSD Tech HM-10 presoldered on breakout board. http://www.dsdtech-global.com/2017/08/hm-10.html

Extensive testing on HM-10s, clones, and counterfeits performed by Martyn Currey at http://www.martyncurrey.com/hm-10-bluetooth-4ble-modules/.

Implementation in this project serves as simple serial monitor forwarding. HM-10 AT commands issued via USB will disconnect any bluetooth LE devices allow configuration of the radio. Otherwise, commands issued via BLE serial terminal will simply forward to the Arduino `Serial` stream and similarly, anything printed OR written to the `Serial` stream will be forward to the radio and printed on the remote BLE terminal. Note `BLE` is not the same as `Bluetooth whatever.0`.

There's a dozen pins on the HM-10 that can *almost* be used as a replacement for something like the Arduino. We can actually write custom values and functions, I believe, to the HM-10 and directly wire our relays to those pins. The breakout board on this DSD-Tech model however, hides those pins.

## Circuit Diagrams
### UNO R3 Pinning
This diagram is more-or-less accurate, but the pinning changed with serial communication requirements and pin interrupt availability. The only physical components missing here are a set of parllel resistors forming a voltage divider (1kΩ-2kΩ) on the Rx pin of the HM-10 BLE radio. The purpose of said voltage divider is to step down the amplitude of serial communication from the Arduino from 5V to 3.3V. It's also worth noting here that pins Tx and Rx pins should be crossed - so the Arduino transmits to the radio's Rx pin, and the radio transmits to the Arduino's Rx pin. There seems to be a lot of confusion about this on the internet, but what I've written here is absolutely correct.

![1](https://user-images.githubusercontent.com/80596121/111043089-8d8d3580-83f5-11eb-9b2e-712eb2f71693.png)

### Backup Power Supply
There isn't exactly a long range monitoring solution here, but in the event you're within BLE radio range, we can at least enter a low-power state and alert a nearby operator that the greenhouse lost power. Anything drawing current through the 5V relays will be shutoff, but at least the system won't undergo a full reboot (losing all runtime configurations).

![2](https://user-images.githubusercontent.com/80596121/111043093-90882600-83f5-11eb-9585-f6b5740079a1.jpg)

## Commands
Commands can be issued from any BLE terminal app. The system is advertised as MAASy and requires a pin/password to connect. The password can only be changed via HM-10 AT commands issued through the Arduino serial monitor on a USB-A connected computer.

Note that AT commands and HM-10 pinning can be found at manufacturer's site, but this Cornell document does a better job of it: https://people.ece.cornell.edu/land/courses/ece4760/PIC32/uart/HM10/DSD%20TECH%20HM-10%20datasheet.pdf. It's also worth referencing Martyn Currey's experiments linked above.

| Command | Description | Syntax |
| :--- | --- | --- |
| `info` | Print a table of all sensor readings and relay states | `info` |
| `time` | Set the internal clock, defaults to compile time on each boot. Use 24-hour time to distinguish AM-PM. | `time <hour> <minute>` |
| `vars` | List the IDs of all changeable cvars | `vars` |
| `get` | Get the value of a cvar | `get <cvar ID>` |
| `set` | Set the value of a cvar | `set <cvar ID> <float>` |

## Classes
### Sensors
`class Sensor`
- to-do
### SR-Latches
`struct Latch`
#### To Do
- Enable, disable latches. Need to be able to shut off pumps without powering down the system. Should also resume it's original state after being re-enabled.
- Make default constructor private and friend it to the `LatchManager` singleton.
#### Description
- A soft implementation of an S-R latch. Basically we take an `e`% error within some control system setpoint value, test our sensor output against this threshold, and open or close a relay to power on an effector and drive the sensor signal in the direction of our desired setpoint.
- Do not use the default constructor. Fill in the blanks during declaration and you're good to go.
#### Behavior
- `e` may be changed at runtime to allow manual and automatic calibration in response to changing environemntal factors such as seasonal snow etc. This value is not to be confused with signal-setpoint error as shown in block diagrams.
- `X` is the control variable value, for instance, humidity. This value is updated by a `Sensor` class at the defined polling rate.
- `S` or `Set` is defined as `X ≤ setpoint*(1-e)`. This represents one half of our feedback calculation.
- `R` or `Reset` is defined as `X ≥ setpoint*(1+e)`. This represents the other half of our feedback calculation.
- `Q̅` is a byte almost exclusively defined as `1` or `HIGH` and represents a constant DC signal to some 5V relay. Equipment drawing current from this relay serves as our effector and needs to be calibrated so as to yield results in a short enough time frame without destabilizing the system. For example, the intensity of a water heater is represented as the water temperature regulator's gain value. 
- `Q` is a byte almost exclusively defined as `0` or `LOW` and also represents a constant DC signal to some 5V relay. Equipment drawing current from this relay is effectively shut down in this state.
#### Notes
- In this specific implementation, it is impossible for both `R` and `S` to be `true`.
![maxresdefault](https://user-images.githubusercontent.com/80596121/111043682-a2b79380-83f8-11eb-90d0-d859a94d069a.jpg)
- Ultimately we should get a relatively stable control system that hovers around our desired setpoints
### Metrics
`class Metric`
- to-do
### Scheduled Tasks
`struct ScheduledTask`
- to-do
