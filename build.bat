@echo off
REM Compilation de la DLL puis du programme de test (MinGW / MSYS2)
g++ -std=c++17 -shared -DBUILD_DLL -o pile.dll pile_dll.cpp -Wl,--out-implib,libpile.a -static-libgcc -static-libstdc++
g++ -std=c++17 main_dll.cpp -o test_dll.exe -L. -lpile -static-libgcc -static-libstdc++
test_dll.exe
pause
