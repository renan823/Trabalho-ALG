all: item.o arb.o avl.o set.o main.o
	gcc -std=c99 -Wall item.o arb.o avl.o set.o main.o -o sets

item.o:
	gcc -c item.c 

arb.o:
	gcc -c arb.c 

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