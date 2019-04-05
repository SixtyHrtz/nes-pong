@echo off

set name=main

cc65 -Oirs src/%name%.c -o %name%.s --add-source

ca65 crt0.s
ca65 %name%.s -g

ld65 -C nrom_32k_vert.cfg -o %name%.nes crt0.o %name%.o nes.lib -Ln labels.txt

del *.o

move /Y labels.txt build/
move /Y %name%.s build/
move /Y %name%.nes build/

start build\%name%.nes
