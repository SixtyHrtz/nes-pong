#!/bin/sh

name="main"

cc65 -Oirs src/$name.c -o $name.s --add-source

ca65 crt0.s
ca65 $name.s -g

ld65 -C nrom_32k_vert.cfg -o $name.nes crt0.o $name.o nes.lib -Ln labels.txt

rm *.o

mv labels.txt build
mv $name.s build
mv $name.nes build

fceux build/$name.nes
