@echo off

cls
cd build
cmake .. -G "MinGW Makefiles"
make
OSUS
cd ..