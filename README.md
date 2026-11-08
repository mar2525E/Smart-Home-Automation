# Smart Home Automation System (ESP32 + Blynk)

## Overview
An IoT smart home system built on an ESP32 that automates outdoor lighting, a corridor light, a fire response fan, a garage door, and a garden irrigation pump, and lets the user control everything remotely from a phone through Blynk. It was my graduation project for my Computer Science degree at AlHadera University. It replaces manual control of home devices with sensor driven automation, which reduces wasted energy and reacts to events like motion or fire without anyone needing to be present.

## Features
- Motion based automation with a PIR sensor: turns on the corridor light when motion is detected in the yard and switches it off 2 seconds after the last motion
- Automatic lighting based on ambient brightness using an LDR: switches the outdoor lights on when it gets dark and off in daylight
- Flame detection: turns on a fan through a relay when fire is detected
- Automatic garage door: an HC-SR04 ultrasonic sensor opens the door when an object is 5-12 cm away, and it closes again after 5 seconds
- 8 relay modules switching the outdoor light, corridor light, fan, 4 lamps, and the water pump
- Servo motor controlling the garage door (also openable from the app)
- Irrigation pump that pumps water from a reservoir through a perforated pipe to the garden
- Remote control and monitoring from the Blynk app (4 lamps, pump, and garage door)

## Hardware
- ESP32 development board
- 8 relay modules
- PIR motion sensor
- LDR (light sensor)
- Flame sensor
- HC-SR04 ultrasonic distance sensor
- Servo motor
- Submersible mini water pump
- External power supply for the pump and relay load side
- Breadboard and jumper wires
- Physical scale model of the house (garage, garden, water reservoir or a pool just to show how it works)

## How it works
The sensors (PIR, LDR, flame, ultrasonic) send their readings to the ESP32, which runs periodic checks using BlynkTimer instead of blocking delays. Based on those readings it switches the relays or moves the servo. The ESP32 also connects to Blynk over WiFi, so the lamps, pump, and garage door can be controlled manually from the phone app.

## Development
Built using the Waterfall methodology in five phases: Planning, Analysis, Design, Implementation, and Testing.

## Results
Successfully presented and defended as my graduation project, with a grade of 97/100. Each component was tested individually and then as a full system using white box, black box, and acceptance testing, and all functions worked as expected.
