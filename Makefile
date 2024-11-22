all: item.o lista.o avl.o set.o main.o
	gcc -std=c99 -Wall item.o lista.o avl.o set.o main.o -o sets

item.o:
	gcc -c item.c 

lista.o:
	gcc -c lista.c 

avl.o:
	gcc -c avl.c 

set.o:
	gcc -c set.c 

main.o:
	gcc -c main.c 

run:
	./sets

clear:
	rm *.o sets