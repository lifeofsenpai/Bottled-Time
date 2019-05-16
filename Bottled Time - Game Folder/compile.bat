c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -c -o main.o main.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -Wf-bo2 -c -o bank2.o bank2.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -Wf-bo3 -c -o bank3.o bank3.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -Wl-yt0x01 -Wl-yo4 -Wl-yp0x143=0x80 -o main.gbc main.o bank2.o bank3.o
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -Wl-yt0x01 -Wl-yo4 -Wl-yp0x143=0x00 -o main.gb main.o bank2.o bank3.o