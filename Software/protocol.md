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
|  Hdr  |  Adr  |  Len  |  RId  |  Cmd  |  Pld  |  CRC  |
|  0x55 |  1B   |  1B   |  1B   |  1B   |  ...  |  1B   |
```

The **Hdr** (Header) field. Size: 1 byte. The very first byte of every package starts with a **Hdr** field which is always `0x55` (`0b01010101`).
The **Adr** (Address) field. Size: 1 byte. Compatible with I2C 7 bit address. The MS bit is reserved and should always be 0.
The **Len** (Length) field. Size: 1 byte. Type: `uint8_t`. The length of the rest of the package including its **CRC** byte.
The **RId** (Request/Response Id) field. Size: 1 byte. Type: `uint8_t`. The unique id of the request that will be repeated by the response. Can be 0.
The **Cmd** (Command) of size 1 byte. Full list of commands is in the *Commands* section.
The **Pld** (Payload) field. Size: 0-26 bytes.
The **CRC** field. The polynomial CRC-8 value of the whole package (excluding this field).

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
The configurable values are persisten memmory cells that are used by the device. Normally stored in EEPROM.

## Dynamic values
The opearating values of the device (e.g. temperature).

## Feather
Device Type Id: `0x01`.
The servo and DC motor controller.

### Configurable values:
- `0x00` - The address of the device. Type: `uint8_t`. Default: `0x10`.
- `0x10` - default value for DC motor on startup. Type: `int8_t`. Default: `0`.
- `0x11` - calibrating minimal value for DC motor. Type: `int8_t`. Default: `-127`.
- `0x12` - calibrating maximal value for DC motor. Type: `int8_t`. Default: `127`.
- `0x13` - calibrating zero value for DC motor. Type: `int8_t`. Default: `0`.
- `0x20` - default value for the servo on startup. Type: `unt8_t`. Default: `0`.
- `0x21` - calibrating minimal value for the servo. Type: `uint8_t`. Default: `0`.
- `0x22` - calibrating maximal value for the servo. Type: `uint8_t`. Default: `255`.

### Dynamic values:
- `0x00` - value of DC motor. Type: `uint8_t`. Defaults to value of `0x00` configurable value.
- `0x01` - DC motor calibrating mode. Type: `int8_t`. Values: `[0, 1]`. When calibrating mode set to `1` the dynamic value `0x00` is not normalized and represents actual value that will be applied to the motor driver. Once this value is changed the actual value of the `0x00` is recalculated according to the mode in order to keep the motor driver at the same value.
- `0x10` - value of servo. Type: `int8_t`. Defaults to value if `0x10` configurable value.
- `0x11` - servo calibrating mode. Type: `int8_t`. Values: `[0, 1]`. When calibrating mode set to `1` the dynamic value `0x10` is not normalized and represents actual value that will be applied to the driver.

## Keel
Device Type Id: `0x02`.
Base communication hub with additional functions.

### Configurable values:
- `0x00` - The address of the device. Type: `uint8_t`. Default: `0x10`.

### Dynamic values:
- `0x00` - value of the battery0. Type: `uint8_t`. Read only (resetting/writing to it has no effect).
- `0x01` - value of the battery1. Type: `uint8_t`. Read only (resetting/writing to it has no effect).
- `0x10` - value of the thermometer0. Type `uint8_t`. Read only (resetting/writing to it has no effect).
