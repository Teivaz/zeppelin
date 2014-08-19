echo off
if "%~x1"==".hex" (
	set FLASH=%~f1
)

if "%~x1"==".eep" (
	set EEPROM=%~f1
)

if not "%~2"=="" (
	if "%~x2"==".hex" (
		set FLASH=%~f2
	)

	if "%~x2"==".eep" (
		set EEPROM=%~f2
	)
)

set command=avrdude - C avrdude.conf -c 2ftbb -p m16 -P ft0 -b 230400 x -B 230400

if not "%EEPROM%"=="" (
	set EEPROM=-U eeprom:w:%EEPROM%:i
)

if not "%FLASH%"=="" (
	set FLASH=-U flash:w:%FLASH%:i
)
%command% %EEPROM% %FLASH%
pause