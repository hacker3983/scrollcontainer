@echo off
gcc -c scroll.c app.c

echo Building scrolling system...
gcc main.c *.o -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf  -o scrollsystem
echo Running scroll system...
scrollsystem.exe
