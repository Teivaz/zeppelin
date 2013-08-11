@ECHO OFF

SET FIRMWARE="UVisionOutput\ZepelinTranssmitter1.bin"
SET DEVICE="STM32F103C8"
SET ADAPTER="Colink"
SET COFLASH="c:\Program Files (x86)\CooCox\CoFlash\coflash.exe"

@ECHO ON
%COFLASH% program -a %ADAPTER% %DEVICE% %FIRMWARE%
