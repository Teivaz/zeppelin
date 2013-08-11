SET PATH=C:\Keil\ARM\ARMCC\bin\;C:\Python33\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\Intel\WiFi\bin\;C:\Program Files\Common Files\Intel\WirelessCommon\;C:\Program Files (x86)\Windows Kits\8.0\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\Program Files (x86)\Intel\OpenCL SDK\3.0\bin\x86;C:\Program Files (x86)\Intel\OpenCL SDK\3.0\bin\x64;C:\Program Files (x86)\MATLAB\R2009b\runtime\win32;C:\Program Files (x86)\MATLAB\R2009b\bin;C:\Program Files D\Autodesk\Backburner\;C:\Program Files (x86)\Common Files\Autodesk Shared\;C:\Program Files D\TortoiseGit\bin;C:\Program Files\Intel\WiFi\bin\;C:\Program Files\Common Files\Intel\WirelessCommon\
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target
SET CPU_CLOCK=0x007A1200
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\uvisionoutput\main.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\uvisionoutput\stm32_init.__i"
"C:\Keil\ARM\ARMCC\bin\ArmAsm" --Via ".\uvisionoutput\stm32f10x._ia"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\uvisionoutput\softspiphy.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\uvisionoutput\spicontrolbridge.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\uvisionoutput\core_cm3.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\uvisionoutput\system_stm32f10x.__i"
"C:\Keil\ARM\ARMCC\bin\ArmLink" --Via ".\UVisionOutput\Nest.lnp"
C:\Keil\ARM\ARMCC\bin\fromelf.exe ".\UVisionOutput\Nest.axf" --i32combined --output ".\UVisionOutput\Nest.hex"
fromelf --bin --output UVisionOutput/Nest.bin UVisionOutput/Nest.axf
