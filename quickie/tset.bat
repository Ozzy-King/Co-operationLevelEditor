@echo off

set /A test = 100
set /A test2 = 101
set /A test3 = 101

if NOT %test% == %test2% goto 1 SETLOCAL
 echo equal
goto 12 
:1
 echo not eqaul
 if NOT %test2% == %test3% goto 2 SETLOCAL
  echo but this is
 ENDLOCAL 
 :2
ENDLOCAL 
:12
