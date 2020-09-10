# Preliminanry Design Review

## Flight Avionics Software
Lead: Austin Bodzas

**Overview**: The scope of this PDR covers all software running onboard the HAB. The flight software contains all of the instructions for capturing data, logging data, actuating the servos, controlling comms and commanding cut-down.

![Software System Interface Overview](img/pdr/sw_sys_interface.png)

Above is the system level interface diagram for the HAB's Flight Software board level components.  The PDR will detail software executing on the Ballon Board, Power Supply Card, Avionics Sensor Card, and Main Flight Computer (MFC).  The Communications card does not require custom new software, and the Ground Computer System

### Main Flight Computer
#### Overview
#### Interfaces
#### Responsibilities
#### Runtime Environment
**IPC**:
#### Applications
##### Altitude Controller
##### System Monitor
##### System Manager
##### Logger 
##### CAN TLM RX
##### CAN TLM TX 
##### ETH TLM RX
##### ETH TLM TX
##### GNU Radio App

## Flight Avionics Hardware 