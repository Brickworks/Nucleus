# Flight Profile

![Flight Profile (Detailed)](img/Flight%20Profile%20Detailed.png)

1.  **Pre-Launch**
    1.  Permits and waivers are acquired.
    2.  Pre-Launch hardware tests pass.
    3.  [Pre-Launch Checklist](placeholder.md)

2.  **Launch**
    1.  [Launch Day Checklist](placeholder.md)
    2.  Remove before flight tags are removed.
    3.  Electronics are powered up.
    4.  Ballast is filled with material.
    5.  Payload enclosure is closed for flight.
    6.  Live telemetry is established with avionics over direct link.
    7.  Health and safety of avionics is observed over live telemetry.
    8.  Balloon is filled with lift gas.
    9.  Balloon is released from ground fixture.

3.  **Ascent Near Range** (0-20,000ft altitude, <25mi slant range)
    1.  Short-range radio link established with ground station receiving live telemetry.
    2.  APRS radio link established with GPS and altitude data.
    3.  Passive ascent to ~20,000ft.

4.  **Ascent Far Range** (20,000-100,000ft, >25mi slant range)
    1.  Long-range radio link established with ground station.
    2.  APRS radio link established with GPS and altitude data.
    3.  Altitude controller armed and waiting to reach the target altitude.

5.  **Stabilize** (target altitude: 80,000 ft)
    1.  The altitude controller begins actuating bleed and ballast valves.
    2.  Ascent rate and altitude change rapidly over time.
    3.  See [Altitude Control System](placeholder.md)

6.  **Cruise** (target altitude: 80,000 ft)
    1.  Altitude is stable (error < 500ft) for an extended period of time.
    2.  Altitude controller is armed and but not active.

7.  **Control Disabled** (Ballast empty or position/time boundary reached)
    1.  All remaining ballast is released.
    2.  Altitude controller is disarmed.

8.  **Cutdown** (position/time boundary reached)
    1.  On-board software determines that the flight must be terminated immediately.
    2.  Multiple (redundant) cutdown elements relieve gas from the balloon.
        1.  Bleed valve remains open until it senses descent.
        2.  Nichrome heating element cuts the latex balloon from the balloon plug.
    3.  [Safety & Abort Systems (Cutdown)](placeholder.md)

9.  **Descent**
    1.  Payload free falls until air pressure is high enough to unfurl the parachute.
    2.  Parachute unfurls and slows descent.
    3.  Payload begins beaconing (light and/or sound indicators).

10.  **Recovery**
    1.  Payload continues to beacon light/sound indicators and GPS location via APRS.
    2.  When landing is detected (altitude does not change for an extended period and controller is disarmed), payload enters low-power state for all systems except recovery beacons.
    3.  Wait for recovery.
