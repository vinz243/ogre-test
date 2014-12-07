@echo off
call "%VS120COMNTOOLS%"\\vsvars32.bat
echo Deleting old exe...
del build_win32\dist\bin\OgreApp.exe
cd build_win32
echo Building As Debug
msbuild /detailedsummary /p:Configuration=Debug /p:Platform=x86 /t:build ALL_BUILD.vcxproj
echo Done building, Exiting.
cd dist\bin\
start OgreApp_d.exe
exit