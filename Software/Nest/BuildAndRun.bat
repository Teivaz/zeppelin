@ECHO OFF
@ECHO =========== Building project ===========
call Target.bat
@ECHO OFF
@ECHO =========== Writting to MCU ===========
call WriteFlash.bat
@ECHO =========== Finished ===========