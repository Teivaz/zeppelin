# Zeppelin <img src="https://raw.githubusercontent.com/Teivaz/zeppelin/master/Blueprints/Nest/icon_large.png" height="40pt"/>

### Airship project with all electrical circuits, PCBs, blueprints, software sources and calculations

### Intended result
First version of this zeppelin should be able to keep itself up in the sky for a decent amount of time. It will be controled remotely (probably via iphone). Total length of airship 168.13cm. Volume 0.46 m3.
In the end it should look like this:    
![zeppelin in all its glory](https://raw.githubusercontent.com/Teivaz/zeppelin/master/Blueprints/Renders/zeppelin_full.png)

### Progress
 - [X] Calculate the overall size, mass, and lifting force of the aerostat
 - [X] Create patterns for aerostat
 - [X] Create electrical schemas
 - [X] Design software architecture
 - [ ] Create and debug airship's software
 - [ ] Create software for controlling unit
 - [ ] Trace PCBs and make all PCBs
 - [ ] Assemble aerostat
 - [ ] Field tests

### Calculations
Here is the [link](https://docs.google.com/spreadsheets/d/1KFVUaK0Eav1PAXg2VrwWtU-BHmDxbaWGcJfdvVmI2J4/edit?usp=sharing) to spreadsheet with calculation I made for the lifting force and mass of the airship 

### Mechanics
Most parts used here were thoroughly measured and calculated. Like these cases for motors and servos (feather module)   
<img alt="preview" src="https://raw.githubusercontent.com/Teivaz/zeppelin/master/Blueprints/Renders/Balloon%20Assembled%20bot.png" width="50%"><img alt="feather" src="https://raw.githubusercontent.com/Teivaz/zeppelin/master/Blueprints/Renders/DC%20Motor%20And%20Servo%20Cover.png" width="50%">

### Electronic schematics and PCBs
Well there's not much to say about schematics despite the fact that I'm using AVR microcontroller attiny25 and transmitting modules RF24013. It is designed and traced with [EAGLE](http://www.cadsoftusa.com).

### Software
Keeping in mind that I am using attiny25 the IDE is obviously [AVR Strudio](http://www.atmel.com/tools/atmelstudio.aspx). The language is C without any fancy libs.

### Version 1.0 Notes
 * Feather MCU should use internal PWM channels directly. This will simplify code and increase working frequency.
 * Feather should use a motor driver that does not short circuit when in low phase of the PWM.
 * Consider using 8 bit controller for the Feather module.

### Further plans
 * Second version will be able to carry a camera.
 * Third version will automatically keep position, folow the route or target.
