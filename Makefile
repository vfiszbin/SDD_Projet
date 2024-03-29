FLAGS = -Wall -Werror -Wextra -g
OPTIONS = -lm -lssl -lcrypto

all : p1main p2main p3main p4main mainfinal
	
p1main : p1main.o p1exo1.o p1exo1.h p1exo2.o p1exo2.h
	
	gcc $(FLAGS) p1main.o p1exo1.o p1exo2.o -o p1main $(OPTIONS)

p1main.o : p1main.c p1exo1.h p1exo2.h
	gcc $(FLAGS) -c p1main.c -o p1main.o 

p2main : p2main.o p1exo1.o p1exo1.h p1exo2.o p1exo2.h p2exo3.o p2exo3.h p2exo4.o p2exo4.h
	gcc $(FLAGS) p2main.o p1exo1.o p1exo2.o p2exo3.o p2exo4.o -o p2main $(OPTIONS)

p2main.o : p2main.c p1exo1.h p1exo2.h p2exo3.h
	gcc $(FLAGS) -c p2main.c -o p2main.o 

p3main : p3main.o p1exo1.o p1exo1.h p1exo2.o p1exo2.h p2exo3.o p2exo3.h p2exo4.o p2exo4.h p3exo5.o p3exo5.h p3exo6.o p3exo6.h
	gcc $(FLAGS) p3main.o p1exo1.o p1exo2.o p2exo3.o p2exo4.o  p3exo5.o p3exo6.o -o p3main $(OPTIONS)

p3main.o : p3main.c p2exo3.h p2exo4.h p3exo5.h
	gcc $(FLAGS) -c p3main.c -o p3main.o 

p4main : p4main.o p1exo1.o p1exo1.h p1exo2.o p1exo2.h p2exo3.o p2exo3.h p2exo4.o p2exo4.h p3exo5.o p3exo5.h p3exo6.o p3exo6.h p4exo7.o p4exo7.h p4exo8.o p4exo8.h p4exo9.o p4exo9.h 
	gcc $(FLAGS) p4main.o p1exo1.o p1exo2.o p2exo3.o p2exo4.o  p3exo5.o p3exo6.o p4exo7.o p4exo8.o p4exo9.o -o p4main $(OPTIONS)

p4main.o : p4main.c p2exo4.h p4exo7.h p4exo8.h p4exo9.h 
	gcc $(FLAGS) -c p4main.c -o p4main.o 

mainfinal : mainfinal.o p1exo1.o p1exo1.h p1exo2.o p1exo2.h p2exo3.o p2exo3.h p2exo4.o p2exo4.h p3exo5.o p3exo5.h p3exo6.o p3exo6.h p4exo7.o p4exo7.h p4exo8.o p4exo8.h p4exo9.o p4exo9.h 
	gcc $(FLAGS) mainfinal.o p1exo1.o p1exo2.o p2exo3.o p2exo4.o  p3exo5.o p3exo6.o p4exo7.o p4exo8.o p4exo9.o -o mainfinal $(OPTIONS)

mainfinal.o : mainfinal.c p2exo3.h p2exo4.h p4exo7.h p4exo8.h p4exo9.h 
	gcc $(FLAGS) -c mainfinal.c -o mainfinal.o 

p1exo1.o : p1exo1.c p1exo1.h
	gcc $(FLAGS) -c p1exo1.c -o p1exo1.o 

p1exo2.o : p1exo2.c p1exo2.h
	gcc $(FLAGS) -c p1exo2.c -o p1exo2.o 

p2exo3.o : p2exo3.c p2exo3.h
	gcc $(FLAGS) -c p2exo3.c -o p2exo3.o 

p2exo4.o : p2exo4.c p2exo4.h
	gcc $(FLAGS) -c p2exo4.c -o p2exo4.o 

p3exo5.o : p3exo5.c p3exo5.h
	gcc $(FLAGS) -c p3exo5.c -o p3exo5.o 

p3exo6.o : p3exo6.c p3exo6.h
	gcc $(FLAGS) -c p3exo6.c -o p3exo6.o 

p4exo7.o : p4exo7.c p4exo7.h
	gcc $(FLAGS) -c p4exo7.c -o p4exo7.o 

p4exo8.o : p4exo8.c p4exo8.h
	gcc $(FLAGS) -c p4exo8.c -o p4exo8.o 

p4exo9.o : p4exo9.c p4exo9.h
	gcc $(FLAGS) -c p4exo9.c -o p4exo9.o 

clean :
	rm -rf *.o
	rm -f p?main
	rm -f mainfinal

	rm -f Blockchain/*
	rm -f blocks.txt
	rm -f candidates.txt
	rm -f declarations.txt
	rm -f keys.txt
	rm -f Pending_block
	rm -f Pending_votes.txt