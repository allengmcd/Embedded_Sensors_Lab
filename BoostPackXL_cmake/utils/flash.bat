::============================================================================
:: Batch file to program to the flash of EK-TM4C123GXL
:: 
:: NOTE: requires the LMFlash programmer (included in QTools for Windows)
:: 
@echo off
setlocal

@echo Load a given binary file to the flash of EK-TM4C129
@echo usage:   flash binary-file
@echo example: flash dbg\blinky-qk.bin

::----------------------------------------------------------------------------
:: NOTE: The following symbol LMFLASH assumes that LMFlash.exe can
:: be found on the PATH. You might need to adjust this symbol to the
:: location of the LMFlash utility on your machine 
::
set LMFLASH=LMFlash.exe

::if ["%~1"]==[""] (
::    @echo The binary file missing
::    @goto end
::)
::if not exist %~s1 (
::    @echo The binary file '%1' does not exist
::    @goto end
::)

%LMFLASH% -q ek-tm4c1294xl -e -v -r ../out/build/arm-gcc-windows/tm4c129xl_project.bin

:end

endlocal