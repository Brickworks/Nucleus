# Flight Hardware

Lead: Matthew Glazer

[TOC]

**Overview**: The scope of this PDR covers all hardware design and development as it currently stands for the Nucleus HAB launch. Development approaches, timelines and overall board information are provided. The design of payload, actuation mechanisms and software design are
discussed in [Altitude Control Logic](pdr_altitudecontrol.md), [Mechanical Systems](placeholder.md) and [Flight Avionics Software](pdr_avsw.md), respectively.

**Overview**: The Flight Hardware system for the nucleus mission provides all the power, computing and control elements required for mission success. There are seven primary PCBs and an assortment of secondary PCBs as part of this system.

### Boards in work:

Backplane Interface Board- Schematic completedMain Flight Computer Card - Template Schematic startedAvionics Sensor Card - Template Schematic startedAltitude Control/Dynamics Sensor Card - Template Schematic startedCommunications Card - Template Schematic startedPower Supply Card - Template Schematic startedBalloon Board

## Design Path Forward

### Commonality

The Main Flight Computer and Payload Cards are based on a template. The MFC Card template approach is more useful for future development of additional missions. However the Payload Card template is the base of all card development. The template will include connector interfaces, switching regulators, communication interfaces and finally the MCU. From here the cards can be tailored to the individual payload cards. 

The intent is to complete this template and then reuse to develop the individual cards, this will reduce development time and provide a basis for future missions.

### Card design

#### Compartmentalization

Continuing the idea of basing each card off a template, the designs are compartmentalized and common across all cards. The goal is for major subsections to be a “copy and paste” approach, i.e. using the same components and schematic blocks wherever possible. Likely schematic blocks include:

* Microcontroller
* 3.3V/1.8V regulators
* Sensors (IMU etc)
* Ethernet/CAN

#### Mechanical

All Cards are designed to roughly the same dimensions, depending on card. Currently all cards are specified to the following dimensions of 100x100mm with a provision to increase to 105x200mm if additional space is needed. There will be three primary “configuration options” for board length, 100mm, 150mm and 200mm. Mounting points will be based off the three configuration options.

All cards have M3 mounting holes 5mm x 5mm from the corner, except for the MFC’s mounting holes by the backplane connector which are 5mm x 15mm from card edge.

Need is unexpected, but any large components would likely be “potted” for strength. 

#### Design Rules

Electrical signals routed on all cards and boards fall into three categories: Differential Routing, GPIO and Power. 

Differential routing will follow Ethernet and CAN design requirements, i.e. 50 or 100 ohm differential pairs etc. These signals will be routed primarily on layer one of the PCB and grouped together away from all power and GPIO signals. A full ground plane will be routed underneath. 

On four layer boards, power will be routed on layer three, on two layer boards power will be routed on layer one. High current paths, such as on the backplane will be implemented through copper pours, while all other routing of power will be through a different trace width versus signals to differentiate trace types.

GPIO will be routed on either layer one or four, depending on keepout zones for the ethernet and CAN routing. Less dependent on a ground layer underneath but all effort will be made to maintain the same style of trace coupling.  

Trace width’s will be dictated by PCB manufacturer’s guidelines. For initial development will likely use the MacroFab design rules guidelines and then tweak as necessary based on final selected vendor.

Positioning is determined by mechanical design’s CAD model as well as card spacing. 

### Common Interfaces

#### Backplane

Interfaces between cards are implemented through Molex Milli-Grid connectors. Backplane mounted connectors are vertical receptacles while card mounted connectors are right angle shrouded connectors. Ground station interface is through a molex MicroFit power header and RJ45 connector. These then attach to a panel mount style interface on outside of HAB enclosure. Balloon board backplane interface runs through a paired JST connector.

#### Cards

External interfaces to the cards will be implemented through JST PH connectors of varying conductor counts. Specifically for the Power Supply Card, a Molex Microfit power header will be used for battery interface. GPS antenna will be an SMA connector interface.

### Manufacturing

#### Board Stackups

* ENIG plating
* Blue soldermask
* White silkscreen
* 1oz copper (outer layers) 
* 0.5oz copper (inner layers)

##### 4 layer PCB 

1. Signal - Differential Pairs
2. Ground
3. Power
4. Signal - GPIO/Ground

##### 2 layer PCB 

1. Signal/Power
2. Ground

#### Card Specifics  

##### Backplane 

PCB design is primarily through hole with some surface mount capacitors. No issues expected with manufacturing. This is the simplest card of the whole stack. Assembly expected to be by hand, by Brickworks members.  

Starting point for stackup: 4 layer PCB

##### Main Flight Computer

If reasonably priced then impedance matching will be added to PCB ordering. 99% surface mount design, only connectors will be through hole. This card is the only card currently being considered for professional manufacturing.

Starting point for stackup: 4 layer PCB

##### Payload cards

99% surface mount design, only connectors will be through hole. No significant impedance match trace work expected, except potentially on the communications Card.

Starting point for stackup: 2 layer PCB

### Battery Pack

Pack will be 18650 cells, these are sourced from matt’s personal stockpile, they are not new manufacture cells but no issues expected as they were salvaged out of a Model S.

#### Pack Design

3.7V nominal; 3100mAh cells. Pack would be configured in a 4 series, 2 parallel (4S2P) design with the option of expanding the packs to larger quantities as mission configuration requires. 

Current mission design is a 14.8V nominal (12-16.8V range), 6200mAh pack. Expected temperatures at altitude will reduce total capacity to a currently unknown amount but a conservative approach is an expectation of 40% total capacity loss at max altitude. This can be accounted for using thermal insulation and potentially a small heater.

#### Manufacturing

Battery pack will be tab welded together for electrical connections, cells well be hotglued together for initial mechanical stability with additional framing to be added as necessary.

Testing

All boards will be designed with large #'s of test points. This points will sit on all voltage rails and major signal paths (where possible). Will allow for testing of most signalling/power, especially useful when testing subsystems such as CAN and Ethernet.

Before power-up, a full continuity test will be performed, checking for opens and shorts where needed/unwanted on all signals and power lines.

Bring-up plan will be power supplies first, testing successful operation and confirming efficiency. Full board assembly will progress after that. Power supplies will be tested for basic output levels, output ripple, load testing and overall efficiency.

### Thermal analysis

All boards will have a power analysis completed to provide input to a thermal simulation. Power dissipation can be calculated across the board with reasonable accuracy combined with known thermal impedance numbers based on vendor information. Using this information we can get an estimate of how the boards will fare temperature wise through the flight. Thermal numbers will be calculated based off a vacuum enviornment as that is "simpler" to generate information for. We do hit near vacuum level enviornments during flight and as such if the calculations/simulations show reasonable operation then this will act as a worst case that shows it will work in other enviornments.

## Major Constraints

Board outline constraints subject to change per CAD model. 

### Backplane

Pinout needs to be determined and set in stone as soon as physically possible.

### Main Flight Computer Card

Heavy focus on proper trace routing for processor and Ethernet/CAN/USB. Processor SoM is a BGA package with wide pin pitch, care needs to be taken during design and manufacturing. Will likely require special constraints for PCB manufacturing.

### Power Supply Card

This card is heavily driven by software, basic functionality requires a software package and cannot run in an “analog” sense. Primarily the battery management system. The switching regulator system will function normally without software function.

### Avionics Sensor Card

GPS system is currently TBD

COTS APRS interface is currently TBD

Recovery assist still needs concept determined to create design. 

### Communications Card

Implementation of this will rely heavily on Dan’s amplifier designs and overall implementation, however major portions have already been decided such as the SDR so work can begin. Block diagram and basic interaction outside of SDR TBD.

## Timeline

### MFC Card Development

3 weeks to complete schematic

3 weeks to complete PCB

3 weeks for manufacturing

### Payload Card development

2 weeks to complete template schematic

1 week to complete customized payload card schematic

3 weeks to complete PCB

3 weeks for manufacturing

### Development plan

I believe this is literally waterfall design. There will be some jumping around between cards as plans are finalized however see below for the approximate development path. The goal will be to simultaneously work on two project points at one point to near completion ( i.e. enough to put down to pick up the next two points. Currently the Backplane schematic is “complete” and work has started on the Backplane PCB and MFC Card schematic. 

![timeline](https://user-images.githubusercontent.com/12124823/93687139-a3d6bc00-fa78-11ea-8a13-1b4806cb1e51.png)

PCB assembly will potentially occur during this development process based on how the card design finalizes and the needs of the software development side of things. However assembly and test of PCBAs is not included in this development schedule. Current expectation is that boards will be ordered and assembled after CDR.

## Cost:

* Backplane - $150 ea
* Main Flight Computer Card - $300 ea
* Avionics Sensor Card - $200 ea
* Altitude Control/Dynamics Sensor Card - $200 ea
* Communications Card - $300 ea
* Power Supply Card - $200 ea
* Balloon Board - $150 ea
Total: $1500 per set

Please note, this is a very rough estimate. The goal is to drive costs lower while maintaining high levels of features (I promise I'm not on the marketing team)

## Card Specific Information

### Backplane

#### CI’s:

* Main Flight Computer Interface
* Payload Card Interfaces
* Communications Card Interface
* Power Supply Card Interface

#### Interfaces:

* Backplane - Ethernet
* Backplane - CAN
* Backplane - USB
* Backplane - Power

### Main Flight Computer Card 

#### Theory of Operation
The Main Flight Computer Card is the brain of the avionics stack. This card handles all primary functions and receives data from all other cards in the stack.


##### Main Processor
An Octavo OSD3358 SoM is the processor on this card. It is a single core application processor that can run linux, providing many options for the software team to develop flight programs. It also contains 512MB of RAM and 4GB of eMMC NAND flash for storage. There are no major external components required to complete the processor design beyond the SOM. The SOM also contains 3.3V and 1.8V regulators on board, simplifying the required external hardware, the SOM accepts 5V input. The SOM interfaces with the Ethernet and CAN drivers through dedicated ports on the processor. 

##### Ethernet Interface
This card contains an Ethernet switch providing access to all payload cards except the power card and balloon board. The switch also interfaces back to the main processor providing the interconnect between all cards. 

##### Secondary Storage
This Card will contain an SD card interface for expanded secondary data storage. 


#### Electrical Interface

##### Major components
Processor - OSD3358 SoM 
CAN Transceiver - TCAN1051

##### Power Supplies

###### 12V Power Rail
No usage

###### 5V Power Rail
* Octavo OSD3358
* SD Card
* Temperature Sensor
* Ethernet Switch


##### Connectors
There are two 34 pin Molex MilliGrid Shrouded Right angle Connectors providing all interfaces to and from the Flight computer to the Backplane and other payload cards.


###### Power/GPIO Connector
Molex Part #: 0878333420

#### CI’s:

* Processor SoM
* Ethernet Switch

#### Interfaces:

* Backplane - Ethernet
* Backplane - CAN
* Backplane - Power

### Power Supply Card

 

#### CI’s:

* Microcontroller
* Switching Regulators
* Battery Pack Charger
* Battery Pack Monitor
* Remove Before Flight Circuit

#### Interfaces:

* Backplane - CAN
* Backplane - Power
* Battery Pack

### Avionics Sensor Card

#### CI’s:

* Microcontroller
* Sensor Network
* GPS system
* Recovery Assist
* Switching Regulators

#### Interfaces:

* Backplane - Ethernet
* Backplane - CAN
* Backplane - Power
* GPS Radio antenna
* Recovery LEDs/Buzzer

### Altitude Control/Dynamics Sensor Card


#### CI’s:

* Microcontroller
* Sensor Network - Altitude Control
* Sensor Network - Dynamics
* Ballast Controller
* Switching Regulators

#### Interfaces:

* Backplane - Ethernet
* Backplane - CAN
* Backplane - Power
* Dynamics Sensor Nodes

### Communications Card


#### CI’s:

* Microcontroller
* Radio Backend
* Radio Front End
* Switching Regulators

#### Interfaces:

* Backplane - Ethernet
* Backplane - CAN
* Backplane - USB
* Backplane - Power
* Radio antennas

### Balloon Board


#### CI’s:

* Microcontroller
* Balloon Monitoring
* Gas Relief System
* Switching Regulators

#### Interfaces:

* Backplane - CAN
* Backplane - Power
* Gas Relief System

