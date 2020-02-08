This document describes the communication protocol for the Zeppelin project.

Project URL: https://github.com/Teivaz/zeppelin
Author: Teivaz
Date: 4.01.2020
Version: 1.0

# Purpose
The communication protocol is used to for commands and data transmition between different parts of the Zeppelin.

# Adopting
This communication protocol is adopted by all parts of the system. Certain parts may not support all of the commands but the package structure should always be supported.

# Architecture
The protocol assumes single master and multiple slave devices.

# Package structure

```
|  Fpr  |  Adr  |  Len  |  RId  |  Cmd  |  Pld  |  CRC  |
|  0x55 |  1B   |  1B   |  1B   |  1B   |  ...  |  1B   |
```

The **Fpr** (Footprint) field. Size: 1 byte. The very first byte of every package starts with a **Fpr** field which is always `0x55` (`0b01010101`).
The **Adr** (Address) field. Size: 1 byte. Compatible with I2C 7 bit address. The MS bit is reserved and should always be 0.
The **Len** (Length) field. Size: 1 byte. Type: `uint8_t`. The length of the rest of the package including its **CRC** byte.
The **RId** (Request/Response Id) field. Size: 1 byte. Type: `uint8_t`. The unique id of the request that will be repeated by the response. Can be 0.
The **Cmd** (Command) of size 1 byte. Full list of commands is in the *Commands* section.
The **Pld** (Payload) field. Size: 0-26 bytes.
The **CRC** field. The (polynomial 0x07, initial value 0x00) CRC-8 value of the whole package (excluding this field).

# Version
The protocol version is `0bMMMMmmmm` - where `M` - major version bits, `m` - minor version bits).

# Commands
Current version of protocol: `0x10`

The commands' most significant bit indicates direction of the command. 0 - request, 1 - response.
*Note* Not all commands have responses.

- `0x00` - *info*. Check presence of the device. Payload length: 0.
- `0x80` - *info-re*. Response to the *info* command. Payload length: 2 bytes. Payload data: 1 byte - device type id; 1 byte - version of the protocol.
- `0x01` - *read-cv* - read configurable value. Payload length: 1 byte. Payload data: 1 byte - id of the configurable value.
- `0x81` - *read-cv-re* - response to the *read-cv* command. Payload length: 2 bytes. Payload data: 1 byte - id of the configurable value; 1 byte - the value.
- `0x02` - *write-cv*. Set a configurable value. Payload length: 2 bytes. Payload data: 1 byte - id of the configurable value (see the list of configurables values for a specific device); 1 byte - the value.
- `0x82` - reserved.
- `0x03` - *reset-cv* - reset configurable value to defaul state. Payload length: 1 byte. Payload data: 1 byte - id of the configurable value.
- `0x83` - reserved.
- `0x04` - *reset-all-cv* - reset all configurable values to defaul state. Payload length: 0.
- `0x84` - reserved.
- `0x05` - *read-dv* - read dynamic value. Payload length: 1 byte. Payload data: 1 byte - id of the dynamic value.
- `0x85` - *read-dv-re* - response to the *read-dv* command. Payload length: 2 bytes. Payload data: 1 byte - id of the ; 1 byte - the value.
- `0x06` - *write-dv* -  Set a dynamic value. Payload length: 2 bytes. Payload data: 1 byte - id of the dynamic value (see the list of dynamic values for a specific device); 1 byte - the value.
- `0x86` - reserved.
- `0x07` - *reset-dv* - reset the dynamic value of the device to default. Payload length: 1 byte - id of the dynamic value.
- `0x87` - reserved.
- `0x08` - *reset-all-dv* - reset all dynamic values of the device to default. Payload length: 0.
- `0x88` - reserved.

# Device descriptions

## Address
Every device needs to have a uniqie address. If there are more than one device with the same address the result is undefined.
The address can be in the range 0x08 - 0x7b.

## Configurable values
The configurable values are persistent memmory cells that are used by the device. Normally stored in EEPROM.

## Dynamic values
The opearating values of the device (e.g. temperature).

## Feather
Device Type Id: `0x01`.
The servo and DC motor controller.

### Configurable values:
- `0x00` - The address of the device. Type: `uint8_t`. Default: `0x10`.
- `0x10` - default value for DC motor on startup. Type: `int8_t`. Default: `0`.
- `0x11` - calibration minimal value for DC motor. Type: `int8_t`. Default: `-128`.
- `0x12` - calibration maximal value for DC motor. Type: `int8_t`. Default: `127`.
- `0x20` - default value for the servo on startup. Type: `uint8_t`. Default: `0`.
- `0x21` - calibration minimal value for the servo. Type: `uint8_t`. Default: `128`.
- `0x22` - calibration maximal value for the servo. Type: `uint8_t`. Default: `128`.

### Dynamic values:
- `0x00` - value of DC motor. Type: `int8_t`. Defaults to value of `0x00` configurable value.
- `0x01` - DC motor calibration mode. Type: `uint8_t`. Values: `[0, 1, 2]`. When calibration mode is set to `0` the device functions normally.
	When calibration mode is set to `1` or `2` the dynamic value `0x00` represents the corresponding calibration value (`0x11` or `0x12` respectively). The DV (dynamic value) is loaded from corresponding CV (configurable value). Value normalization is disabled.
	**Note** Writing to this value would not change the value of any CV. To save the calibration result - read the DV `0x00` and set corresponding CV with *write-cv* command.
	When calibration mode is set back to `0` the value of DV `0x00` is reset to its default.
- `0x10` - value of servo. Type: `uint8_t`. Defaults to value if `0x10` configurable value.
- `0x11` - servo calibration mode. Type: `uint8_t`. Values: `[0, 1, 2]`. When calibration mode is set to `0` the device functions normally.
	When calibration mode is set to `1` or `2` the dynamic value `0x10` represents the corresponding calibration value (`0x21` or `0x22` respectively). The DV is loaded from corresponding CV. Value normalization is disabled.
	**Note** Writing to this value would not change the value of any CV. To save the calibration result - read the DV `0x10` and set corresponding CV with *write-cv* command.

## Keel
Device Type Id: `0x02`.
Base communication hub with additional functions.

### Configurable values:
- `0x00` - The address of the device. Type: `uint8_t`. Default: `0x10`.

### Dynamic values:
- `0x00` - value of the battery0. Type: `uint8_t`. Read only (resetting/writing to it has no effect). The value is in decivolts.
- `0x01` - value of the battery1. Type: `uint8_t`. Read only (resetting/writing to it has no effect). The value is in decivolts.
- `0x10` - value of the temperature0. Type `int8_t`. Read only (resetting/writing to it has no effect). The value is in degrees Celcius.
