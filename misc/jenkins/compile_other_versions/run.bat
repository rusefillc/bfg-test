set script_name=run.bat
echo Entering %script_name%

set FTP_SERVER=home451478433.1and1-data.host

pwd

call misc\jenkins\build_java_console.bat
if not exist java_console_binary/rusefi_console.jar exit -1


set bundle_name=stm32f746_nucleo
call misc\jenkins\compile_other_versions\compile_and_upload.bat nucleo_f746 stm32f746_nucleo
IF NOT ERRORLEVEL 0 echo ERROR invoking compile_and_upload.bat
IF NOT ERRORLEVEL 0 EXIT /B 1
pwd
call misc\jenkins\compile_other_versions\build_version.bat
echo %script_name%: Done with F746-nucleo


set bundle_name=stm32f767_nucleo
call misc\jenkins\compile_other_versions\compile_and_upload.bat nucleo_f767 stm32f767_nucleo
IF NOT ERRORLEVEL 0 echo ERROR invoking compile_and_upload.bat
IF NOT ERRORLEVEL 0 EXIT /B 1
pwd
call misc\jenkins\compile_other_versions\build_version.bat
echo %script_name%: Done with F767-nucleo


set bundle_name=stm32f767_osc
call misc\jenkins\compile_other_versions\compile_and_upload.bat nucleo_f767 stm32f767_osc
IF NOT ERRORLEVEL 0 echo ERROR invoking compile_and_upload.bat
IF NOT ERRORLEVEL 0 EXIT /B 1
pwd
pwd
call misc\jenkins\compile_other_versions\build_version.bat
echo %script_name%: Done with F767-osc



cd firmware
call clean.bat
cd ..
echo %script_name%: Compiling prometheus_405
cd firmware\config\boards\prometheus
call !compile-prometheus-405.bat
if not exist build/rusefi.hex echo Just to confirm - FAILED to compile prometheus-405
if not exist build/rusefi.hex exit -1
cd ..
pwd
set bundle_name=prometheus_405
call misc\jenkins\compile_other_versions\build_version.bat
echo %script_name%: Done with prometheus_405



cd firmware
call clean.bat
cd ..
echo %script_name%: Compiling prometheus_469
cd firmware\config\boards\prometheus
call !compile-prometheus-469.bat
if not exist build/rusefi.hex echo Just to confirm - FAILED to compile prometheus-469
if not exist build/rusefi.hex exit -1
cd ..
pwd
set bundle_name=prometheus_469
call misc\jenkins\compile_other_versions\build_version.bat
echo %script_name%: Done with prometheus_469


