




app.exe: menu.o wave.o main.o
	gcc -o menu.o -c menu.c	
	gcc -o wave.o -c wave.c	
	gcc -o args.o -c args.c
	gcc -o main.o -c main.c -lm
	gcc -o app.exe menu.o wave.o args.o main.o


clean: 
	rm *.o
	rm app.exe

