echo off
set PATH=%PATH%;../../Software/tool/

set EEPROM=ATTiny25-'Z1'.eep

upload_attiny25 ATTiny25-Feather.hex %EEPROM%