all: item.o lista.o ab.o set.o main.o
	gcc -std=c99 -Wall item.o lista.o ab.o set.o main.o -o sets

item.o:
	gcc -c item.c 

lista.o:
	gcc -c lista.c 

ab.o:
	gcc -c ab.c 

set.o:
	gcc -c set.c 

main.o:
	gcc -c main.c 

run:
	./sets

clear:
	rm *.o sets