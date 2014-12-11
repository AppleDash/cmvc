all: cmvc

cmvc: linkedlist.o util.o viewhandler.o cgi.o main.o
	gcc -o main linkedlist.o util.o viewhandler.o cgi.o main.o

main.o: main.c
	gcc -c main.c

cgi.o: cgi.c
	gcc -c cgi.c

viewhandler.o: viewhandler.c
	gcc -c viewhandler.c

util.o: util.c
	gcc -c util.c

linkedlist.o: adlib/linkedlist.c
	gcc -c adlib/linkedlist.c