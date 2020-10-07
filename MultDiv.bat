@echo off
bash -c "rm MultDiv.o; echo Compiling MultDiv.cpp; g++ MultDiv.cpp -o MultDiv.o -std=c++11; echo Running executable; ./MultDiv.o"