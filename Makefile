CC=gcc.exe
CFLAGS=-O2 -std=c99
EXEC=oxymetre.exe

all: exec


main.o: main.c
	gcc -c -Wall main.c

fichiers.o: fichiers.c
	gcc -c -Wall fichiers.c

lecture.o: lecture.c
	gcc -c -Wall lecture.c -lm ftd2xx.lib

fir.o: fir.c
	gcc -c -Wall fir.c

iir.o: iir.c
	gcc -c -Wall iir.c

mesure.o: mesure.c
	gcc -c -Wall mesure.c

affichage.o: affichage.c
	gcc -c -Wall affichage.c

integration.o: integration.c
	gcc -c -Wall integration.c


exec: affichage.o fichiers.o fir.o iir.o integration.o lecture.o main.o mesure.o ftd2xx.lib
	gcc -o oxymetre.exe affichage.o fichiers.o fir.o iir.o integration.o lecture.o main.o mesure.o ftd2xx.lib -O2 -std=c90

clean:
	rm -f *.o