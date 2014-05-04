@echo off
robocopy ./build/shared ./ 2> nul > nul
robocopy ./build/windows ./ 2> nul > nul
..\MCPP\gcc48\bin\mingw32-make.exe %*