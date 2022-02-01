@echo off

cls
cd build\desktop
cmake ../.. -G "MinGW Makefiles"
make
cd ../..
build\desktop\OSUS