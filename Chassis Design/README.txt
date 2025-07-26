# Line Follower Robot (LFR) Chassis Design

This repository contains the 3D CAD design of a Line Follower Robot (LFR) chassis with integrated mounts for motors and sensors, along with structural reinforcements for durability. The design is optimized for 3D printing and supports modular electronics placement, making it suitable for competition-grade and academic robotics projects.

## Features

- **Motor Mounts**:
  - Compatible with **25GA370 DC gear motors** (2x)
  - Motors are secured using **2 x M3x6 mm screws** each
  - The motor mounting structure is fixed to the chassis using **4 x M4 screws**
- **Ultrasonic Sensor Mounts**:
  - Slots for **three HC-SR04 ultrasonic sensors**:
    - Front-facing
    - Left-facing
    - Right-facing
  - Secured using **M3 screws**
- **IR Sensor Mount**:
  - Designed for the **TechTopia IR Sensor Array**
  - Mounted with **2 x M3 screws**
  - Sensor reference: [TechTopia IR Sensor Array](https://techtopiabd.com/product/ir-sensor-array/?srsltid=AfmBOopA3Qe3Gx2WuvtD4BUmO_z58qM0nOmTy6Itx8Fhr6Guj2j0Q0mJ)
- **Caster Mounts**:
  - Two front cavities for **N20 ball casters**, to be attached with glue or double-sided tape
- **Structural Support**:
  - Reinforcement beams behind the motor zone increase chassis rigidity
- **Electronics Mounting Area**:
  - Flat platform in the center for microcontroller/PCB mounting

Model available on GrabCAD:  
[GrabCAD – LFR Chassis with Integrated Sensor & Motor Mounts](https://grabcad.com/library/line-follower-robot-lfr-chassis-with-integrated-sensor-motor-mounts-1)

## Applications

- Line follower robotics competitions
- Path-finding and obstacle-avoiding robots
- Autonomous navigation systems
- Educational and prototyping platforms for embedded systems

## Recommended Components

| Component             | Description                                      |
|----------------------|--------------------------------------------------|
| Motors               | 25GA370 DC gear motors (x2)                      |
| Ultrasonic Sensors   | HC-SR04 (x3 — front, left, right)                |
| IR Sensor            | TechTopia IR Sensor Array                        |
| Ball Casters         | N20 ball casters (x2)                            |
| Controller           | Arduino Uno / Nano / STM32                       |
| Power Supply         | 7.4V Li-ion / 12V battery                        |
| Screws               | M3 x 6 mm (motor to mount)                       |
|                      | M4 (mount to chassis)                            |
|                      | M3 (IR and HC-SR04 sensors)                      |

## Assembly Notes

- Secure each 25GA370 motor to its mount using 2 x M3x6 mm screws.
- Mounts are then fixed to the chassis using 4 x M4 screws per motor.
- Each HC-SR04 ultrasonic sensor fits into a designated slot and is secured using M3 screws.
- The IR sensor is mounted underneath the chassis using 2 x M3 screws.
- N20 ball casters fit into the front cavities and should be attached using strong adhesive or double-sided foam tape.
- The chassis has sufficient flat surface area to mount a microcontroller board using screws or Velcro.

## 3D Printing Guidelines

- Material: PLA or PETG
- Infill: 20-25%
- Supports: Needed 




