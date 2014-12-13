@echo off
setlocal
call "%VS120COMNTOOLS%"\\vsvars32.bat
echo Deleting old exe...
del build_win32\dist\bin\OgreApp_d.exe
cd build_win32
echo Building As Debug
chcp 850
msbuild /detailedsummary /p:Configuration=Debug /p:Platform=x86 /t:build ALL_BUILD.vcxproj
echo Done building, Exiting.
cd dist\bin\
OgreApp_d.exe
cd ../../../
endlocal