C=gcc

my_app: main.o text.o
	$(C) main.o text.o -o my_app

main.o: main.c
	$(C) main.c -c

text.o: text.c
	$(C) text.c -c

clean:
	rm *.o my_app
