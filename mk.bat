@echo off

call Compile Rasteriser.c
if NOT %errorlevel%==0 ( goto END )

call Compile Vector.c
if NOT %errorlevel%==0 ( goto END )

call Compile Matrix.c
if NOT %errorlevel%==0 ( goto END )

call Compile SOLID.c
if NOT %errorlevel%==0 ( goto END )

call Compile Maths.c
if NOT %errorlevel%==0 ( goto END )

call Compile Graphics.c
if NOT %errorlevel%==0 ( goto END )

call Compile Controller.c
if NOT %errorlevel%==0 ( goto END )

call Compile Object.c
if NOT %errorlevel%==0 ( goto END )

call Compile Power.c
if NOT %errorlevel%==0 ( goto END )

call Compile Plasma.c
if NOT %errorlevel%==0 ( goto END )

call Compile Floor.c
if NOT %errorlevel%==0 ( goto END )

c:\ps2dev\gcc\ee\bin\ee-gcc -Wall -march=r4000 -g -mgp32 -c -xassembler -O -o startup.o startup.s
if NOT %errorlevel%==0 ( goto END )

echo Linking...
C:\PS2Dev\gcc\ee\bin\ee-ld startup.o SOLID.o Graphics.o Plasma.o Floor.o Object.o Controller.o Maths.o Vector.o Matrix.o Power.o Rasteriser.o -M -Ttext 8900000 -q -o out > SOLID.map
if NOT %errorlevel%==0 ( goto END )
echo Linked!

outpatch
elf2pbp outp "Solid"

:END
