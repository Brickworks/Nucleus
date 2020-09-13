# Altitude Control Logic

Lead: Philip Linden

Jump to:

* [Concept of Operations](#concept-of-operations)
* [PID Control](#pid-control)
* [Avionics](#avionics)
* [System Design Parameters]()
* [Risk Assessment]()

See also:

* [Main Flight Computer](../pdr_avsw.md#main-flight-computer-software-design)
* [Avionics Sensor Card](../pdr_avsw.md#avionics-sensor-card-software-design)
* [Balloon Board](../pdr_avsw.md#balloon-board-software-design)
* [Altitude Control/Dynamics
  Card](../pdr_avsw.md#altitude-controldynamics-card-software-design)
* [Balloon Bleed Valve]()
* [Ballast Hopper]()

**Overview**: The scope of this PDR covers the logic and operations of the
altitude controller system on the HAB, including arm and disarm criteria,
approach for calculating when to actuate controllers, and risk mitigation
strategies. The design of actuation mechanisms and electronic components are
discussed in [Mechanical Systems](./pdr_mechanical.md) and [Flight Avionics
Hardware](./pdr_avhw.md), respectively.

**Objective**: Actively control the weight and buoyancy of the HAB in order to
maintain equilibrium at a prescribed altitude.

![Mission Profile Crop](img/pdr/altitudecontrol/mission-profile_altctrl.png)

## Approach
Normally the balloon is filled with lifting gas so that it rises with the
payload until it reaches an altitude where the ambient temperature and pressure
cause the balloon to expand beyond its limits and it bursts. Without active
control, flight time can be increased by filling the balloon with just enough
lift gas to be at equilibrium at the target altitude, but this is difficult for
several reasons:

- The actual temperature and pressure at the target altitude are at best
  roughly approximated by models, even when accounting for the latest weather
  forecast.
- Temperature and pressure are not constant at the target altitude, so even if
  there was a perfectly measured amount of lift gas to be at equilibrium for a
  specific temperature and pressure band, it wouldn’t follow a target altitude.
- Filling the balloon with less gas would mean it takes longer for the HAB to
  reach the target altitude.

The HAB’s altitude is a function of two opposing forces: the upward buoyancy
force and the weight of the balloon-payload system. Critically, both are
directly influenced by something we can control. Buoyancy is a function of the
amount of lift gas in the balloon, and weight is a function of the mass of the
payload. Thus we can design a closed-loop control algorithm to change the
balance of these two forces by bleeding lift gas from the balloon or dropping
ballast mass from the payload based on the measured altitude in order to
converge to some target altitude. This addresses a number of issues encountered
with passive flight:

- Dynamic feedback control allows the balloon to maintain equilibrium at a
  target altitude through changing temperatures and pressures.
- Open-loop control of lifting gas allows the balloon to be overfilled at
  launch, rising to the target altitude quickly and then bleeding gas from the
  balloon until equilibrium is achieved.

![Difference in approaches](img/pdr/altitudecontrol/passive-vs-active-ctrl.png)

In order for the controller to converge to a geometric altitude, it must use a
non-atmospheric reference like GPS altitude. Barometric altimeters measure
pressure, which is calibrated to isobaric layers of a standard reference
atmosphere.

**Primary mode**: GPS altitude hold. <br/> **Secondary mode**: Isobaric
altitude hold.

## Concept of Operations

1. Wait for a *controller enable* to be sent.
    1. Disarm the controller when the avionics boot up.
    2. During pre-launch, operators enable the controller.
2. Disarm the controller unless when all of the following conditions are met:
    1. A controller enable command has been received since the last boot or the
       last controller disable command.
    2. The control error has crossed zero at least once (i.e., the HAB’s
       altitude has risen above the target altitude at least once).
    3. There is ballast mass remaining in the reservoir.
        1. Calibrate the mass flow rate at the ballast release valve prior to
           flight and keep a tally of remaining ballast mass.
   1. There is enough lifting gas in the balloon for the HAB to rise if the
      reservoir were empty.
      1. Use a pressure sensor in the balloon to compute the mass flow rate at
         the bleed valve from the gauge pressure and temperature of lifting gas
         inside the balloon and keep a tally of remaining lifting gas.
3. Use a tuned PI control to drive a ballast release valve and a gas bleed
   valve. The controller is designed for disturbance rejection of a noisy
   altitude signal. <br/> ![Bleed Ballast
   Ops](img/pdr/altitudecontrol/bleedballast-ops.png)
    1. In order to conserve control effort, we must tune the controller such
       that overshoot and oscillations are minimized.
    2. Response time and settling time should be tuned to be small compared to
       flight time, but can still be on the order of ~1 hour.
    3. Control loop PWM frequency is tuned to be much faster than the response
       time.
4. **GPS altitude hold (Stabilize):** Enter stabilize control mode when the
   controller is armed and remain in this mode until ascent rate and/or
   altitude error are greater than the allowable bounds.
    1. Use GPS altitude to converge to a geometric altitude that may cross
       isobaric layer boundaries.
    2. This control mode uses more control mass than barometric altitude hold
       so it should not be used for extended periods of time.
    3. Use this control mode during the stabilize phase to quickly approach the
       target altitude.
5. **Barometric altitude hold (Cruise):** Once the vehicle is near the target
   altitude and has reached a steady state, transition to cruise mode to
   maintain an altitude near the target for an extended period of time.
    1. Use barometric altitude to equalize to an isobaric layer of the
       atmosphere corresponding to a calibrated approximate altitude.
    2. This control mode uses less control mass because it is able to equalize
       the balloon pressure to ambient pressure and “ride” isobaric layers of
       atmosphere.
    3. Hold an isobaric altitude during the cruise phase in order to conserve
       control mass.
6. If the controller is still armed and the altitude error exceeds the
   allowable range, transition from cruise to stabilize and then repeat this
   algorithm until the controller is disarmed.
7. When the controller is disarmed, dump all remaining ballast.
    1. Lock the ballast valve in the open position.
    2. Lock the bleed valve in the closed position.

## PID Control
In order to dynamically determine how much to bleed or ballast, we implement a
PID controller to determine the percentage of time of each control cycle to
keep the bleed or ballast valve open. This is also referred to as PWM control
with a PID algorithm.

The PID control algorithm works by modulating the inputs, u, to the Plant,
which in our case means the bleed and ballast controls. The Plant is the
physical response, so the balance of weight, buoyancy, and drag forces with the
given mass properties and ambient conditions. (See [Simple Atmospheric Flight
Model](../atmospheric_flight_model.md)) The output of the Plant is the actual
altitude, *y*. For a given set point altitude, *r*, the PID controller attempts
to reduce the error, *e=r-y*, to zero.

The PID controller consists of a set of gains applied to a proportional term,
an integral term, and a derivative term that all operate on the error signal,
*e*. These gains are tuned to get the desired response from the controller.

### Rules of thumb

**Proportional gain**: How aggressively signal approaches setpoint. Too much causes overshoot. No impact on steady-state error.

**Integral gain**: How much error matters over time. Reduces oscillations and steady-state errors. Prone to windup, where accumulated errors over a long time cause lag in the controller after it reaches the desired point.

**Derivative gain**: How much rate of change matters. Reduces overshoot. Prone to oscillations. Noisy signals make it go crazy, but this can be mitigated by adding a lowpass filter.

![PID Diagram](img/pdr/altitudecontrol/pid-general-diagram.png)
![PID Tuning Example 1](img/pdr/altitudecontrol/pid-tuning-example-1.png) ![PID Tuning Example 2](img/pdr/altitudecontrol/pid-tuning-example-2.png)

### References
- [BYU Dynamics and Control by John D. Hedengren](http://apmonitor.com/pdc/index.php/Main/ProportionalIntegralControl)
- [Control Tutorials for Matlab & Simulink](http://ctms.engin.umich.edu/CTMS/index.php?example=Introduction&section=ControlPID)
- [PID Control Design by A.B. Campo](https://www.intechopen.com/books/matlab-a-fundamental-tool-for-scientific-computing-and-engineering-applications-volume-1/pid-control-design)
- [Wikipedia - PID Controller, Wikipedia - Integral Windup](https://en.wikipedia.org/wiki/Integral_windup)
- [The Basics of Tuning PID Loops](https://www.crossco.com/blog/basics-tuning-pid-loops/)

### Tunable Parameters
To achieve the desired behavior from the controller, we tune some of the parameters in software and tune other parameters as part of choosing or sizing elements of the system during the design process.

#### Pure Tunables
I call the parameters that we can choose and change “for free” pure tunables, since tuning these values can be done as desired without extra engineering work.

- Target altitude
- Controller dead zone (allowable steady state error)
- Control loop cycle time (compute)
- Control loop cycle time (actuation PWM)
- Sensor measurement sample time
- Lowpass filter gain
- PID controller gains (which determine response time and percent overshoot)
- Initial ballast mass
- Initial lift gas mass
- Lift gas reserve fraction (fraction of total mass of lift gas allocated for altitude control)
- Arm / disarm criteria

#### Calibrated Tunables
*Calibrated* tunables are what I call parameters that affect the controller’s behavior or system response but can’t be dialed in on a whim. These are things that require extra engineering or design changes to tune. We choose ideal values for these parameters during the design process, and then we measure the true values when the system is built.

- Ballast mass flow rate
- Bleed mass flow rate
- Actuator and component dry mass
- Maximum control loop cycle time (compute)
- Maximum control loop cycle time (actuation PWM)
- Maximum sensor measurement sample time
- Maximum lift gas reserve fraction

## Avionics
The altitude control flight software application ([Main Flight Computer Software Design](../pdr_avsw.md#main-flight-computer-software-design)) issues commands to open the ballast or bleed valves over the CAN bus. When a bleed or ballast command is received by the altitude control interface board ([Altitude Control/Dynamics Sensor Card](../pdr_avsw.md#avionics-sensor-card-software-design)) over CAN, it executes the corresponding subroutine to carry out the command. The software environment onboard the altitude control interface board (Altitude Control/Dynamics Card Software Design) includes drivers for the hardware interfaces, such as a PWM motor controller for the valve actuators. 

The altitude control interface board is also responsible for polling sensors and actuators for telemetry and pre-processing the raw signals (e.g. voltages from a pressure sensor) into the appropriate derived values (e.g. values in units of pressure) before sending them to the CAN bus for use by other flight software and downlink.

![Flight Software Diagram](img/pdr/altitudecontrol/fsw-diagram.png)

### Hardware
